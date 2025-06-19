#include "app.h"

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Web Crawler App", wxDefaultPosition, wxSize(1024, 720))
{
    mainPanel = new wxPanel(this);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // App Logog
    SetIcon(wxIcon(logo_xpm));

    // App Title
    wxStaticText *title = new wxStaticText(mainPanel, wxID_ANY, "Web Crawler App", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    title->Wrap(1000);
    mainSizer->Add(title, 0, wxALIGN_CENTER | wxALL, 10);

    // App Description
    wxStaticText *desc = new wxStaticText(mainPanel, wxID_ANY, "Click below to crawl a website, scrape its HTML content, and parse it.");
    desc->Wrap(800);
    mainSizer->Add(desc, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    wxPanel* controlsPanel = new wxPanel(mainPanel);
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxVERTICAL);

    // Fetch Button
    fetchButton = new wxButton(controlsPanel, ID_FETCH, "Fetch Website Data");
    controlsSizer->Add(fetchButton, 0, wxALIGN_CENTER | wxALL, 10);
    Bind(wxEVT_BUTTON, &MyFrame::OnFetchBooks, this, ID_FETCH);

    // Loading Text (hidden by default)
    loadingText = new wxStaticText(controlsPanel, wxID_ANY, "Fetching data, please wait...");
    loadingText->Hide();
    controlsSizer->Add(loadingText, 0, wxALIGN_CENTER | wxALL, 10);

    // Add gauge loader
    m_loadingGauge = new wxGauge(controlsPanel, wxID_ANY, 100, wxDefaultPosition, wxSize(300, 20));
    m_loadingGauge->SetForegroundColour(*wxGREEN);
    m_loadingGauge->Hide();
    controlsSizer->Add(m_loadingGauge, 0, wxALIGN_CENTER | wxALL, 5);

    controlsPanel->SetSizer(controlsSizer);
    mainSizer->Add(controlsPanel, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    // Scrollable Panel for Books (hidden until data is fetched)
    scrollPanel = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrollPanel->SetScrollRate(10, 10);
    scrollPanel->Hide();

    wrapSizer = new wxWrapSizer(wxHORIZONTAL);
    scrollPanel->SetSizer(wrapSizer);

    mainSizer->Add(scrollPanel, 1, wxEXPAND | wxALL, 10);

    mainPanel->SetSizer(mainSizer);
}

void MyFrame::OnFetchBooks(wxCommandEvent &event)
{
    fetchButton->Disable();
    loadingText->Show();
    m_loadingGauge->Show();
    scrollPanel->Hide();
    mainPanel->Layout();
    mainPanel->Refresh();
    wxYieldIfNeeded();

    std::thread([this]()
    {
        std::string html = fetchHTML("https://books.toscrape.com/");
        std::vector<Category> categories = parseCategories(html);
        int totalCategories = static_cast<int>(categories.size());

        categoriesFetched = 0;

        wxTheApp->CallAfter([this, totalCategories]() {
            m_loadingGauge->SetRange(totalCategories);
            m_loadingGauge->SetValue(0);
            m_loadingGauge->Show();
            loadingText->SetLabel(wxString::Format("Found %d categories, starting...", totalCategories));
            mainPanel->Layout();
        });

        std::vector<Book> allBooks;
        #pragma omp parallel
        {
            std::vector<Book> localBooks;
            #pragma omp for nowait schedule(dynamic)
            for (int i = 0; i < categories.size(); ++i)
            {
                try
                {
                    std::string catHtml = fetchHTML(categories[i].url);
                    std::vector<Book> books = parseBooks(catHtml, categories[i]);
                    #pragma omp critical
                    {
                        allBooks.insert(allBooks.end(), books.begin(), books.end());
                    }
                }
                catch (...)
                {
                    // Handle or skip
                }
                categoriesFetched++;
                wxTheApp->CallAfter([this, totalCategories]() {
                    m_loadingGauge->SetValue(categoriesFetched);
                    loadingText->SetLabel(wxString::Format("Processing... %d / %d", categoriesFetched.load(), totalCategories));
                });
            }
        }
        wxTheApp->CallAfter([this, totalCategories, allBooks]()
        {
            m_loadingGauge->SetValue(totalCategories);
            loadingText->SetLabel("Loading complete!");
            mainPanel->Layout();
            wxMilliSleep(300);

            m_loadingGauge->Hide();
            loadingText->Hide();
            fetchButton->Enable();
            if(totalCategories < 1)
                wxMessageBox(std::string("No category found!"), "Issue", wxOK | wxICON_WARNING);
            ShowBooks(allBooks);
        });
    })
    .detach();
}

void MyFrame::ShowBooks(const std::vector<Book> &books)
{
    wrapSizer->Clear(true); // remove previous items if any

    ImageLoader& loader = ImageLoader::getInstance();

    for (size_t i = 0; i < books.size(); i++)
    {
        const Book &book = books[i];
        wxPanel *bookPanel = new wxPanel(scrollPanel, wxID_ANY, wxDefaultPosition, wxSize(220, 320));
        wxBoxSizer *bookSizer = new wxBoxSizer(wxVERTICAL);

        // Display Image
        wxImage placeholder(150, 200);
        placeholder.SetRGB(wxRect(0, 0, 150, 200), 230, 230, 230);
        wxStaticBitmap* imageCtrl = new wxStaticBitmap(bookPanel, wxID_ANY, wxBitmap(placeholder));
        imageCtrl->SetName(wxString::Format("image_%zu", i));
        imageCtrl->SetClientData(new std::string(book.imageUrl));
        bookSizer->Add(imageCtrl, 0, wxALIGN_CENTER | wxALL, 5);

        // Start downloading image in background thread
        loader.enqueue([this, imageCtrl, url = book.imageUrl]() {
            auto data = fetchBinary(url);
            auto bitmap = ImageLoader::processImageData(data);
            if(bitmap.IsOk()) {
                wxTheApp->CallAfter([imageCtrl, bitmap]() {
                    imageCtrl->SetBitmap(bitmap);
                    imageCtrl->Refresh();
                    // Clean up client data if you set it
                    if(auto* urlPtr = static_cast<std::string*>(imageCtrl->GetClientData())) {
                        delete urlPtr;
                        imageCtrl->SetClientData(nullptr);
                    }
                });
            }
        });

        wxStaticText *titleText = new wxStaticText(bookPanel, wxID_ANY, wxString::FromUTF8(book.title.c_str()), wxDefaultPosition, wxSize(300, -1), wxALIGN_CENTER);
        titleText->Wrap(300);
        bookSizer->Add(titleText, 0, wxALIGN_CENTER, 0);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Category: " + book.category.name), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Price: " +  wxString::FromUTF8(book.price.c_str())), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Rating: " + book.rating), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "In Stock: " + book.availability), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxHyperlinkCtrl(bookPanel, wxID_ANY, "View Book", book.link), 0, wxALIGN_CENTER, 0);

        bookPanel->SetSizer(bookSizer);
        wrapSizer->Add(bookPanel, 0, wxALL, 10);
    }

    scrollPanel->FitInside();
    scrollPanel->SetScrollRate(10, 10);

    scrollPanel->Layout();
    scrollPanel->Show(true);

    mainSizer->Layout();
    mainPanel->Layout();
    mainPanel->Refresh();
    mainPanel->Update();

    loadingText->Hide();
    fetchButton->Enable();
}

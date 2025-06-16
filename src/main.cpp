#include <vector>
#include <string>

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/hyperlink.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/wrapsizer.h>

#include "includes/book.h"
#include "includes/fetch.h"
#include "includes/parser.h"
#include "assets/logo.xpm"

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnFetchBooks(wxCommandEvent &event);
    void ShowBooks(const std::vector<Book> &books);
    wxBitmap LoadImageFromUrl(const std::string &url);

    wxPanel *mainPanel;
    wxBoxSizer *mainSizer;
    wxStaticText *loadingText;
    wxScrolledWindow *scrollPanel;
    wxWrapSizer *wrapSizer;
    wxButton *fetchButton;
};

enum
{
    ID_FETCH = wxID_HIGHEST + 1
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Books to Scrape", wxDefaultPosition, wxSize(1024, 720))
{
    mainPanel = new wxPanel(this);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // App Logog
    SetIcon(wxIcon(logo_xpm));

    // App Title
    wxStaticText *title = new wxStaticText(mainPanel, wxID_ANY, "Books to Scrape", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    title->SetFont(wxFont(22, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    title->Wrap(1000);
    mainSizer->Add(title, 0, wxALIGN_CENTER | wxALL, 10);

    // App Description
    wxStaticText *desc = new wxStaticText(mainPanel, wxID_ANY, "Click below to fetch books from the online bookstore and browse them beautifully.");
    desc->Wrap(800);
    mainSizer->Add(desc, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    // Fetch Button
    fetchButton = new wxButton(mainPanel, ID_FETCH, "Fetch Books");
    mainSizer->Add(fetchButton, 0, wxALIGN_CENTER | wxALL, 10);
    Bind(wxEVT_BUTTON, &MyFrame::OnFetchBooks, this, ID_FETCH);

    // Loading Text (hidden by default)
    loadingText = new wxStaticText(mainPanel, wxID_ANY, "Fetching books, please wait...");
    loadingText->Hide();
    mainSizer->Add(loadingText, 0, wxALIGN_CENTER | wxALL, 10);

    // Scrollable Panel for Books (hidden until data is fetched)
    scrollPanel = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrollPanel->SetScrollRate(10, 10);
    scrollPanel->Hide();

    wrapSizer = new wxWrapSizer(wxHORIZONTAL);
    scrollPanel->SetSizer(wrapSizer);

    mainSizer->Add(scrollPanel, 1, wxEXPAND | wxALL, 10);

    mainPanel->SetSizer(mainSizer);
}

void MyFrame::ShowCategories(const std::vector<Category> &categories)
{
    wxPanel *sidePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, -1));
    wxBoxSizer *sideSizer = new wxBoxSizer(wxVERTICAL);

    sideSizer->Add(new wxStaticText(sidePanel, wxID_ANY, "Categories"), 0, wxALL | wxALIGN_CENTER, 10);

    for (const Category &cat : categories)
    {
        wxHyperlinkCtrl *link = new wxHyperlinkCtrl(sidePanel, wxID_ANY, cat.name, "https://books.toscrape.com/" + cat.url);
        sideSizer->Add(link, 0, wxLEFT | wxRIGHT | wxTOP, 5);
    }

    sidePanel->SetSizer(sideSizer);

    // Add to main sizer (assuming a main horizontal sizer)
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(sidePanel, 0, wxEXPAND);
    wxPanel *booksPanel = new wxPanel(this);
    booksPanel->SetMinSize(wxSize(800, -1)); // leave space for grid
    mainSizer->Add(booksPanel, 1, wxEXPAND);

    SetSizer(mainSizer);
}

void MyFrame::OnFetchBooks(wxCommandEvent &event)
{
    fetchButton->Disable();
    loadingText->Show();
    scrollPanel->Hide();
    Layout();
    Refresh();

    // Simulate fetch delay using CallAfter
    wxYield();

    std::string html = fetchHTML("https://books.toscrape.com/");
    std::vector<Book> books = parseBooks(html);

    // ShowBooks(books);
    loadingText->Hide();
    scrollPanel->Show();
    fetchButton->Enable();
    Layout();
    Refresh();
}

void MyFrame::ShowBooks(const std::vector<Book> &books)
{
    wrapSizer->Clear(true); // remove previous items if any

    for (const Book &book : books)
    {
        wxPanel *bookPanel = new wxPanel(scrollPanel, wxID_ANY, wxDefaultPosition, wxSize(220, 320));
        wxBoxSizer *bookSizer = new wxBoxSizer(wxVERTICAL);

        wxBitmap bitmap = LoadImageFromUrl(book.imageUrl);
        wxStaticBitmap *imageCtrl = new wxStaticBitmap(bookPanel, wxID_ANY, bitmap, wxDefaultPosition, wxSize(150, 200));

        wxStaticText *titleText = new wxStaticText(bookPanel, wxID_ANY, book.title);
        titleText->Wrap(200);

        bookSizer->Add(imageCtrl, 0, wxALIGN_CENTER | wxALL, 5);
        bookSizer->Add(titleText, 0, wxALIGN_CENTER | wxBOTTOM, 5);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Price: " + book.price), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Rating: " + book.rating), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Stock: " + book.availability), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxHyperlinkCtrl(bookPanel, wxID_ANY, "View Book", book.link), 0, wxALIGN_CENTER | wxTOP, 5);

        bookPanel->SetSizer(bookSizer);
        wrapSizer->Add(bookPanel, 0, wxALL, 10);
    }

    scrollPanel->FitInside();
    scrollPanel->Layout();
}

wxBitmap MyFrame::LoadImageFromUrl(const std::string &url)
{
    wxImage image;
    try
    {
        std::vector<char> raw = fetchBinary(url);
        wxMemoryInputStream stream(raw.data(), raw.size());
        if (!image.LoadFile(stream) || !image.IsOk())
            throw std::runtime_error("Failed to load");
    }
    catch (...)
    {
        image = wxImage(150, 200);
        image.SetRGB(wxRect(0, 0, 150, 200), 230, 230, 230);
    }

    return wxBitmap(image.Scale(150, 200));
}

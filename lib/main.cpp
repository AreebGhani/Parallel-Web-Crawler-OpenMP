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

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const std::vector<Book> &books);

private:
    void ShowBooks(const std::vector<Book> &books);
    wxBitmap LoadImageFromUrl(const std::string &url);
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    std::string html = fetchHTML("https://books.toscrape.com/");
    std::vector<Book> books = parseBooks(html);

    MyFrame *frame = new MyFrame(books);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const std::vector<Book> &books)
    : wxFrame(nullptr, wxID_ANY, "Books to Scrape", wxDefaultPosition, wxSize(1000, 700))
{
    ShowBooks(books);
}

void MyFrame::ShowBooks(const std::vector<Book> &books)
{
    wxScrolledWindow *scrollPanel = new wxScrolledWindow(this, wxID_ANY);
    scrollPanel->SetScrollRate(10, 10);

    wxWrapSizer *wrapSizer = new wxWrapSizer(wxHORIZONTAL);

    for (const Book &book : books)
    {
        wxPanel *bookPanel = new wxPanel(scrollPanel, wxID_ANY, wxDefaultPosition, wxSize(220, 300));
        wxBoxSizer *bookSizer = new wxBoxSizer(wxVERTICAL);

        // Load image
        wxBitmap bitmap = LoadImageFromUrl("https://books.toscrape.com/" + book.imageUrl);
        wxStaticBitmap *imageCtrl = new wxStaticBitmap(bookPanel, wxID_ANY, bitmap, wxDefaultPosition, wxSize(150, 200));

        bookSizer->Add(imageCtrl, 0, wxALIGN_CENTER | wxALL, 5);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, book.title), 0, wxALIGN_CENTER | wxBOTTOM, 2);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Price: " + book.price), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Rating: " + book.rating), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxStaticText(bookPanel, wxID_ANY, "Stock: " + book.availability), 0, wxALIGN_CENTER);
        bookSizer->Add(new wxHyperlinkCtrl(bookPanel, wxID_ANY, "View", "https://books.toscrape.com/" + book.link), 0, wxALIGN_CENTER | wxTOP, 5);

        bookPanel->SetSizer(bookSizer);
        wrapSizer->Add(bookPanel, 0, wxALL, 10);
    }

    scrollPanel->SetSizer(wrapSizer);
}

wxBitmap MyFrame::LoadImageFromUrl(const std::string &url)
{
    wxImage image;
    try
    {
        std::vector<char> raw = fetchBinary(url);
        wxMemoryInputStream stream(raw.data(), raw.size());

        if (!image.LoadFile(stream, wxBITMAP_TYPE_JPEG) || !image.IsOk())
        {
            throw std::runtime_error("Failed to load image");
        }
    }
    catch (...)
    {
        image = wxImage(150, 200);
        image.SetRGB(wxRect(0, 0, 150, 200), 220, 220, 220);
    }

    return wxBitmap(image.Scale(150, 200));
}

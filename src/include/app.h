#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <omp.h>

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/hyperlink.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/wrapsizer.h>
#include <wx/bitmap.h>
#include <wx/msgdlg.h>
#include <wx/gauge.h>
#include <wx/timer.h>

#include "book.h"
#include "fetch_data.h"
#include "parser_html.h"
#include "image_loader.h"
#include "assets/logo.xpm"

enum
{
    ID_FETCH = wxID_HIGHEST + 1
};

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
    wxPanel *mainPanel;
    wxBoxSizer *mainSizer;
    wxScrolledWindow *scrollPanel;
    wxWrapSizer *wrapSizer;
    wxButton *fetchButton;
    wxStaticText *loadingText;
    wxGauge *m_loadingGauge;
    std::atomic<int> categoriesFetched = 0;

    void OnFetchBooks(wxCommandEvent &event);
    void ShowBooks(const std::vector<Book> &books);
};

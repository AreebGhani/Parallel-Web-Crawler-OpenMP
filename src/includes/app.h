#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <omp.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

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

#include "book.h"
#include "fetch_html.h"
#include "parser_html.h"
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
    void OnFetchBooks(wxCommandEvent &event);
    void ShowBooks(const std::vector<Book> &books);
    wxPanel *mainPanel;
    wxBoxSizer *mainSizer;
    wxStaticText *loadingText;
    wxScrolledWindow *scrollPanel;
    wxWrapSizer *wrapSizer;
    wxButton *fetchButton;
};

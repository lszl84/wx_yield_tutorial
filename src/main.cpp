#include <wx/wx.h>
#include <chrono>
#include <memory>

#include "chunkedsort.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    virtual ~MyFrame() {}

private:
    wxButton *button;
    wxButton *otherButton;

    wxStaticText *label;
    wxGauge *progressBar;

    std::unique_ptr<ChunkedSort<int>> sortOp;
    std::chrono::steady_clock::time_point startTime;

    void OnButtonClick(wxCommandEvent &e);
    void OnIdle(wxIdleEvent &e);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxDefaultSize);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *centeringSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, this->FromDIP(wxSize(320, 240)));
    wxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

    label = new wxStaticText(panel, wxID_ANY, "Click Start", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    progressBar = new wxGauge(panel, wxID_ANY, 1000);

    button = new wxButton(panel, wxID_ANY, "Start");
    button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this);

    otherButton = new wxButton(panel, wxID_ANY, "Dummy Button");

    panelSizer->Add(label, 1, wxEXPAND | wxALL, this->FromDIP(20));
    panelSizer->Add(progressBar, 1, wxEXPAND | wxALL, this->FromDIP(20));
    panelSizer->Add(button, 0, wxALIGN_CENTER | wxALL, this->FromDIP(5));
    panelSizer->Add(otherButton, 0, wxALIGN_CENTER | wxBOTTOM, this->FromDIP(20));

    panel->SetSizer(panelSizer);

    centeringSizer->Add(panel, 0, wxALIGN_CENTER);
    sizer->Add(centeringSizer, 1, wxALIGN_CENTER);

    this->SetSizerAndFit(sizer);
}

void MyFrame::OnButtonClick(wxCommandEvent &e)
{
    if (!this->sortOp)
    {
        std::vector<int> arr(50000, 5);
        arr.back() = 3;

        this->sortOp = std::make_unique<ChunkedSort<int>>(std::move(arr));

        this->label->SetLabelText(wxString::Format("Sorting the array of %zu elements...", this->sortOp->arr.size()));
        this->Layout();

        startTime = std::chrono::steady_clock::now();
        this->Bind(wxEVT_IDLE, &MyFrame::OnIdle, this);
    }
}

void MyFrame::OnIdle(wxIdleEvent &e)
{
    if (this->sortOp)
    {
        if (this->sortOp->finished())
        {
            auto endTime = std::chrono::steady_clock::now();
            auto diff = endTime - startTime;
            this->label->SetLabelText(wxString::Format("The first number is: %d.\nProcessing time: %.2f [ms]", this->sortOp->arr.front(), std::chrono::duration<double, std::milli>(diff).count()));
            this->Layout();

            this->Unbind(wxEVT_IDLE, &MyFrame::OnIdle, this);
            this->sortOp = {};
        }
        else
        {
            this->sortOp->processChunk();
            this->progressBar->SetValue(this->sortOp->getProgress(this->progressBar->GetRange()));

            e.RequestMore();
        }
    }
}

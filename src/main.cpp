#include <wx/wx.h>

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

    void OnButtonClick(wxCommandEvent &e);
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

    button = new wxButton(panel, wxID_ANY, "Start");
    button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this);

    otherButton = new wxButton(panel, wxID_ANY, "Dummy Button");

    panelSizer->Add(label, 1, wxEXPAND | wxALL, this->FromDIP(20));
    panelSizer->Add(button, 0, wxALIGN_CENTER | wxALL, this->FromDIP(5));
    panelSizer->Add(otherButton, 0, wxALIGN_CENTER | wxBOTTOM, this->FromDIP(20));

    panel->SetSizer(panelSizer);

    centeringSizer->Add(panel, 0, wxALIGN_CENTER);
    sizer->Add(centeringSizer, 1, wxALIGN_CENTER);

    this->SetSizerAndFit(sizer);
}

void MyFrame::OnButtonClick(wxCommandEvent &e)
{
    std::vector<int> arr(50000, 5);
    arr.back() = 3;

    int n = arr.size();

    this->label->SetLabelText(wxString::Format("Sorting the array of %d elements...", n));
    this->Layout();

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    this->label->SetLabelText(wxString::Format("The first number is: %d", arr.front()));
    this->Layout();
}
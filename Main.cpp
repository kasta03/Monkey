#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbmp.h> 
#include <wx/filedlg.h>
#include <wx/file.h>
#include <wx/gauge.h>
#include <wx/colordlg.h>
#include <string>
#include <wx/colour.h> 
using namespace std;
float promien = sqrt(200 * 60 + 50 * 50), up;

class MyPanel : public wxPanel
{

    wxPoint center;
public:
    MyPanel(wxWindow* parent) : wxPanel(parent)
    {
        Bind(wxEVT_PAINT, &MyPanel::Paint, this);
        Bind(wxEVT_SIZE, &MyPanel::OnSize, this);

        wxImage image;
        if (image.LoadFile("banan.png", wxBITMAP_TYPE_PNG)) {
            bitmapa = new wxBitmap(image);
        }
        else {
            wxLogError("Nie można wczytać obrazu PNG.");
            return;
        }
    }
    int currFigure = 0;
    int smile = 0; 
    wxColour color_star = *wxBLACK; 
    wxString tekst = "Jak malpa"; 
    int banan_pozycja = 0;

    wxBitmap* bitmapa;

    void OnSize(wxSizeEvent& event) {
        wxSize size = GetSize();
        center.x = size.GetWidth() / 2;
        center.y = size.GetHeight() / 2;
        Refresh();
    }
    void Paint(wxPaintEvent& event)
    {

        wxPen transparentPen(*wxTRANSPARENT_PEN);
        wxBrush brush(color_star, wxBRUSHSTYLE_SOLID);
        wxPen pen(*wxBLACK, 1);
        wxPaintDC dc(this);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
        dc.Clear();
        wxString textToDraw = tekst;
        wxString textToDraw_2 = tekst;
        double maxAngle = M_PI / 3.0;
        double currentAngle = maxAngle * banan_pozycja / 100;
        int x = center.x - 100;
        int y = center.y - 100;
        int size = 50;
        switch (currFigure) {
        case 0: {
            // rysowanie gwaidki

            wxPoint points[10];
            float angle = 0;
            float angleIncrement = 2 * M_PI / 10;
            float outerRadius = 50;
            float innerRadius = 20;
            for (int i = 0; i < 10; ++i) {
                float x1 = x + outerRadius * cos(angle);
                float y1 = y + outerRadius * sin(angle);
                points[i] = wxPoint(x1, y1);
                angle += angleIncrement;
                float x2 = x + innerRadius * cos(angle);
                float y2 = y + innerRadius * sin(angle);
                points[++i] = wxPoint(x2, y2);
                angle += angleIncrement;
            }
            // Rysowanie gwiazdki z wyznaczonych punktów
            dc.SetBrush(brush); // Ustawienie koloru gwiazdki
            dc.DrawPolygon(10, points);
            break;
        }
        case 1: { // Księżyc
            // Rysowanie księżyca w lewym górnym rogu
            dc.SetBrush(*wxGREY_BRUSH);
            dc.DrawCircle(x, y, size);
            dc.SetBrush(*wxWHITE_BRUSH);
            dc.DrawCircle(x + 20, y - 20, size);
            break;
        }
        case 2: { // Słońce
            // Rysowanie słońca w lewym górnym rogu
            dc.SetBrush(*wxYELLOW_BRUSH);
            dc.DrawCircle(x, y, size);
            break;
        }
        }

        dc.SetPen(pen);
        dc.SetBrush(*wxWHITE_BRUSH);


        // rysowanie małpy
        float pointx = center.x - promien * sin(currentAngle + M_PI / 4);
        float pointy = center.y + promien * cos(currentAngle + M_PI / 4);

        dc.DrawLine(center.x, center.y, center.x + promien * sin(M_PI / 4), center.y + promien * cos(M_PI / 4));
        dc.DrawLine(pointx, pointy, center.x, center.y);
        dc.DrawLine(center.x, center.y - 20, center.x, center.y + 100);
        dc.DrawLine(center.x, center.y + 100, center.x + 40, center.y + 160);
        dc.DrawLine(center.x, center.y + 100, center.x - 40, center.y + 160);
        dc.DrawCircle(center.x, center.y - 40, 20);
        dc.DrawEllipse(center.x - 15, center.y - 45, 5, 5);
        if (!smile) {
            dc.DrawEllipticArc(center.x - 12, center.y - 30, 25, 20, 50, 140);
            dc.DrawEllipse(center.x + 5, center.y - 45, 5, 5);
        }
        else {
            dc.DrawEllipticArc(center.x - 12, center.y - 45, 25, 20, 220, 320);
            dc.DrawEllipse(center.x + 5, center.y - 45, 5, 5);
            dc.DrawBitmap(*bitmapa, pointx - 20, pointy - 30, true);
        }
        dc.DrawText(textToDraw, center.x - 100, center.y + 150);

        dc.SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false));
        dc.DrawRotatedText(textToDraw_2, center.x + 60, center.y + 170, 90);
    }

};
class MyFrame : public wxFrame
{
    MyPanel* myPanel;
    wxCheckBox* banan;
    wxScrollBar* scrollBar;
    wxGauge* gauge;
    wxButton* star_color;
    wxTextCtrl* text;
    wxArrayString choices;
    wxChoice* choice;
public:
    MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900, 600))
    {
        choices.Add(wxT("gwiazdka"));
        choices.Add(wxT("księżyc"));
        choices.Add(wxT("słonko"));
        myPanel = new MyPanel(this);
        myPanel->Refresh();
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        banan = new wxCheckBox(this, ID_banana, wxT("Banan"));
        scrollBar = new wxScrollBar(this, ID_angle);
        scrollBar->SetScrollbar(0, 1, 100, 1);
        gauge = new wxGauge(this, ID_gauge, 100);
        star_color = new wxButton(this, ID_star, wxT("kolor gwiazdki"), wxDefaultPosition, wxDefaultSize);
        text = new wxTextCtrl(this, ID_text, "tekst", wxDefaultPosition, wxDefaultSize, 0);
        choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
        choice->SetSelection(0);
        sizer->Add(new wxButton(this, ID_save_file, wxT("Zapisz Plik")), 1, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
        sizer->Add(banan, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
        sizer->Add(gauge, 1, wxEXPAND | wxTOP | wxBOTTOM, 11);
        sizer->Add(scrollBar, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
        sizer->Add(star_color, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
        sizer->Add(text, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
        sizer->Add(choice, 1, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 10);
        wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

        mainSizer->Add(myPanel, 7, wxEXPAND | wxALL, 5);
        mainSizer->Add(sizer, 3, wxALIGN_TOP | wxALL, 5);

        SetSizerAndFit(mainSizer);
        scrollBar->Disable();
        Bind(wxEVT_BUTTON, &MyFrame::save_file, this, ID_save_file);
        banan->Bind(wxEVT_CHECKBOX, &MyFrame::onBananaCheck, this);
        scrollBar->Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnScroll, this);
        scrollBar->Bind(wxEVT_SCROLL_CHANGED, &MyFrame::OnScroll, this);
        star_color->Bind(wxEVT_BUTTON, &MyFrame::OnColorChoose, this);
        text->Bind(wxEVT_TEXT, &MyFrame::OnTextEnter, this);
        choice->Bind(wxEVT_CHOICE, &MyFrame::OnChoiceMade, this);
        SetIcon(wxNullIcon);

        Centre();
        Show(true);
    }


    void onBananaCheck(wxCommandEvent& e) {
        if (e.IsChecked()) {
            scrollBar->Enable();
            myPanel->smile = 1;
        }
        else {
            scrollBar->Disable();
            myPanel->smile = 0;
        }
        myPanel->Refresh();
    }
    void OnScroll(wxScrollEvent& event) {
        int position = event.GetPosition();
        gauge->SetValue(position);
        myPanel->banan_pozycja = position;
        myPanel->Refresh();
    }
    void OnColorChoose(wxCommandEvent& event)
    {
        wxColourDialog dialog(this);
        if (dialog.ShowModal() == wxID_OK)
        {
            wxColour colour = dialog.GetColourData().GetColour();
            myPanel->color_star = colour;
            myPanel->Refresh();
        }

    }
    void OnTextEnter(wxCommandEvent& event) {
        myPanel->tekst = event.GetString();
        myPanel->Refresh();
    }
    void OnChoiceMade(wxCommandEvent& event) {
        myPanel->currFigure = event.GetSelection();
        myPanel->Refresh();
    }
    enum
    {
        ID_save_file = 1,
        ID_banana,
        ID_angle,
        ID_gauge,
        ID_star,
        ID_text,
        ID_choice
    };


    void save_file(wxCommandEvent& e) {
        wxClientDC dc(myPanel);
        wxSize panelSize = myPanel->GetSize();
        wxBitmap to_save(panelSize);

        wxMemoryDC memDC;
        memDC.SelectObject(to_save);

        memDC.Blit(0, 0, panelSize.GetWidth(), panelSize.GetHeight(), &dc, 0, 0);

        wxString filename = wxFileSelector("Save As", "", "", "png", "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
        if (!filename.empty()) 
        {
            wxBitmapType type = wxBITMAP_TYPE_BMP; // Format obrazu (PNG)
            to_save.SaveFile(filename, type);
        }
    }

};
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};
bool MyApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler);

    SetProcessDPIAware();
    MyFrame* frame = new MyFrame("Gra");
    frame->Show(true);
    return true;
}
wxIMPLEMENT_APP(MyApp);
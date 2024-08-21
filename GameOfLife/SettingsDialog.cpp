#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize),
    settings(settings)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Grid Size Setting
    wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
    gridSizeSizer->Add(new wxStaticText(this, wxID_ANY, "Grid Size:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY);
    gridSizeCtrl->SetRange(5, 100);
    gridSizeCtrl->SetValue(settings->gridSize);
    gridSizeSizer->Add(gridSizeCtrl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(gridSizeSizer, 0, wxEXPAND | wxALL, 5);

    // Interval Setting
    wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
    intervalSizer->Add(new wxStaticText(this, wxID_ANY, "Interval (ms):"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    intervalCtrl = new wxSpinCtrl(this, wxID_ANY);
    intervalCtrl->SetRange(10, 10000);
    intervalCtrl->SetValue(settings->interval);
    intervalSizer->Add(intervalCtrl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(intervalSizer, 0, wxEXPAND | wxALL, 5);

    // Living Cell Color Picker
    wxBoxSizer* livingCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
    livingCellColorSizer->Add(new wxStaticText(this, wxID_ANY, "Living Cell Color:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    livingCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetLivingCellColor());
    livingCellColorSizer->Add(livingCellColorPicker, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(livingCellColorSizer, 0, wxEXPAND | wxALL, 5);

    // Dead Cell Color Picker
    wxBoxSizer* deadCellColorSizer = new wxBoxSizer(wxHORIZONTAL);
    deadCellColorSizer->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Color:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    deadCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, settings->GetDeadCellColor());
    deadCellColorSizer->Add(deadCellColorPicker, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    mainSizer->Add(deadCellColorSizer, 0, wxEXPAND | wxALL, 5);

    // OK and Cancel Buttons
    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(mainSizer);
}

void SettingsDialog::OnOk(wxCommandEvent& event)
{
    settings->gridSize = gridSizeCtrl->GetValue();
    settings->interval = intervalCtrl->GetValue();
    settings->SetLivingCellColor(livingCellColorPicker->GetColour());
    settings->SetDeadCellColor(deadCellColorPicker->GetColour());

    EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

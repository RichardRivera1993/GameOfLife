#include "MainWindow.h"

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400))
{
    drawingPanel = new DrawingPanel(this);

    sizer = new wxBoxSizer(wxVERTICAL); // Instantiate wxBoxSizer with wxVERTICAL
    sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0); // Add DrawingPanel to sizer with expand and proportion

    this->SetSizer(sizer); // Set the sizer for the window

    this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this); // Bind resize event
}

MainWindow::~MainWindow()
{
    // Cleanup code, if needed
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
    wxSize size = this->GetSize(); // Get the new size of the window
    drawingPanel->SetSize(size);   // Set the size for the DrawingPanel
    event.Skip(); // Allow other events to be processed
}

#include "MainWindow.h"

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400))
{
    drawingPanel = new DrawingPanel(this);
}

MainWindow::~MainWindow()
{
    // Cleanup code goes here
}

#include "MainWindow.h"

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400))
{
    InitializeGrid(); // Initialize the game board before creating the DrawingPanel

    drawingPanel = new DrawingPanel(this, gameBoard); // Pass the game board reference

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);

    this->SetSizer(sizer);
    this->Bind(wxEVT_SIZE, &MainWindow::OnSizeChange, this);
}

MainWindow::~MainWindow()
{
    // Cleanup code, if needed
}

void MainWindow::InitializeGrid()
{
    gameBoard.resize(gridSize);
    for (int i = 0; i < gridSize; ++i)
    {
        gameBoard[i].resize(gridSize, false); // Initialize all cells as dead (false)
    }

    if (drawingPanel) {
        drawingPanel->SetGridSize(gridSize); // Ensure drawingPanel is not nullptr
    }
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
    wxSize size = this->GetSize(); // Get the new size of the window
    drawingPanel->SetSize(size);   // Set the size for the DrawingPanel
    event.Skip(); // Allow other events to be processed
}

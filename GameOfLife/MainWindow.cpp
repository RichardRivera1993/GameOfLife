#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChange)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400))
{
    InitializeGrid(); // Initialize the game board

    drawingPanel = new DrawingPanel(this, gameBoard); // Initialize DrawingPanel after gameBoard

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);

    this->SetSizer(sizer);

    // Create the status bar
    statusBar = this->CreateStatusBar();
    UpdateStatusBar(); // Update status bar with initial values

    this->Layout(); // Ensure the layout includes the status bar
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
        drawingPanel->SetGridSize(gridSize);// Pass the grid size to the DrawingPanel
    }
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
    wxSize size = this->GetSize();
    drawingPanel->SetSize(size);
    event.Skip();
}

void MainWindow::UpdateStatusBar()
{
    wxString statusText;
    statusText.Printf("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
    statusBar->SetStatusText(statusText);
}

#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChange)
EVT_MENU(ID_Play, MainWindow::OnPlay)
EVT_MENU(ID_Pause, MainWindow::OnPause)
EVT_MENU(ID_Next, MainWindow::OnNext)
EVT_MENU(ID_Clear, MainWindow::OnClear)
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

    // Create the toolbar
    toolBar = this->CreateToolBar();

    // Add toolbar buttons
    wxBitmap playIcon(play_xpm);
    toolBar->AddTool(ID_Play, "Play", playIcon);

    wxBitmap pauseIcon(pause_xpm);
    toolBar->AddTool(ID_Pause, "Pause", pauseIcon);

    wxBitmap nextIcon(next_xpm);
    toolBar->AddTool(ID_Next, "Next", nextIcon);

    wxBitmap clearIcon(trash_xpm);
    toolBar->AddTool(ID_Clear, "Clear", clearIcon);

    // Render the toolbar
    toolBar->Realize();

    this->Layout(); // Ensure the layout includes the status bar and toolbar
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
    if (drawingPanel) {
        wxSize size = this->GetSize();
        drawingPanel->SetSize(size);
    }
    event.Skip();
}

void MainWindow::UpdateStatusBar()
{
    wxString statusText;
    statusText.Printf("Generations: %d | Living Cells: %d", generationCount, livingCellsCount);
    statusBar->SetStatusText(statusText);
}
// Event handler implementations
void MainWindow::OnPlay(wxCommandEvent& event)
{
    // Start the game logic
}

void MainWindow::OnPause(wxCommandEvent& event)
{
    // Pause the game logic
}

void MainWindow::OnNext(wxCommandEvent& event)
{
    // Advance the game by one generation
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    // Clear the game board
    InitializeGrid(); // Re-initialize the grid to clear it
    drawingPanel->Refresh(); // Refresh the drawing panel to reflect the change
}
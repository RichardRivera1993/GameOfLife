#include "MainWindow.h"
#include "SettingsDialog.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <wx/numdlg.h> // this is for wxGetNumberFromUser


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::OnSizeChange)
EVT_MENU(ID_Play, MainWindow::OnPlay)
EVT_MENU(ID_Pause, MainWindow::OnPause)
EVT_MENU(ID_Next, MainWindow::OnNext)
EVT_MENU(ID_Clear, MainWindow::OnClear)
EVT_MENU(ID_Settings, MainWindow::OnSettings)
EVT_MENU(ID_ShowNeighborCount, MainWindow::OnToggleShowNeighborCount)
EVT_MENU(ID_Randomize, MainWindow::OnRandomize)
EVT_MENU(ID_RandomizeWithSeed, MainWindow::OnRandomizeWithSeed)
EVT_TIMER(wxID_ANY, MainWindow::OnTimer)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400)),
    timer(new wxTimer(this, wxID_ANY))
{
    settings.Load();  // Load settings when the program starts
    InitializeGrid();

    drawingPanel = new DrawingPanel(this, gameBoard);
    drawingPanel->SetSettings(&settings);

    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(drawingPanel, 1, wxEXPAND | wxALL, 0);

    this->SetSizer(sizer);

    statusBar = this->CreateStatusBar();
    UpdateStatusBar();

    toolBar = this->CreateToolBar();
    toolBar->AddTool(ID_Play, "Play", wxBitmap(play_xpm));
    toolBar->AddTool(ID_Pause, "Pause", wxBitmap(pause_xpm));
    toolBar->AddTool(ID_Next, "Next", wxBitmap(next_xpm));
    toolBar->AddTool(ID_Clear, "Clear", wxBitmap(trash_xpm));

    toolBar->Realize();

    // Create and configure the menu bar
    menuBar = new wxMenuBar();

    // Options Menu
    optionsMenu = new wxMenu();
    optionsMenu->Append(ID_Settings, "&Settings...\tCtrl-S", "Configure settings");
    optionsMenu->Append(ID_Randomize, "&Randomize Grid", "Fill the grid with random cells");
    optionsMenu->Append(ID_RandomizeWithSeed, "&Randomize Grid with Seed...", "Fill the grid with random cells using a specific seed");
    menuBar->Append(optionsMenu, "&Options");

    // View Menu (for Show Neighbor Count)
    wxMenu* viewMenu = new wxMenu();
    viewMenu->AppendCheckItem(ID_ShowNeighborCount, "Show Neighbor Count", "Show number of living neighbors");
    viewMenu->Check(ID_ShowNeighborCount, settings.showNeighborCount);  // Set initial check state
    menuBar->Append(viewMenu, "&View");

    // Set the menu bar
    SetMenuBar(menuBar);

    this->Layout();
}

MainWindow::~MainWindow()
{
    if (timer) {
        timer->Stop();
        delete timer;
    }
}

void MainWindow::InitializeGrid()
{
    gameBoard.resize(settings.gridSize);
    neighborCounts.resize(settings.gridSize);
    for (int i = 0; i < settings.gridSize; ++i)
    {
        gameBoard[i].resize(settings.gridSize, false);
        neighborCounts[i].resize(settings.gridSize, 0);
    }
}

void MainWindow::OnSettings(wxCommandEvent& event)
{
    SettingsDialog dialog(this, &settings);
    Settings oldSettings = settings;  // Backup the current settings

    if (dialog.ShowModal() == wxID_OK)
    {
        settings.Save();  // Save settings if the user clicks OK
        InitializeGrid();
        drawingPanel->SetSettings(&settings);
        drawingPanel->Refresh();

        if (timer->IsRunning()) {
            timer->Start(settings.interval);
        }
    }
    else
    {
        settings = oldSettings;  // Restore old settings if the user clicks Cancel
    }
}

void MainWindow::OnToggleShowNeighborCount(wxCommandEvent& event)
{
    settings.showNeighborCount = !settings.showNeighborCount;
    settings.Save();  // Save the updated setting
    drawingPanel->Refresh();  // Redraw the panel to reflect the new setting
}

void MainWindow::OnRandomize(wxCommandEvent& event)
{
    RandomizeGrid(static_cast<int>(time(NULL)));
}

void MainWindow::OnRandomizeWithSeed(wxCommandEvent& event)
{
    long seed = wxGetNumberFromUser(
        "Enter a seed value for randomization",
        "Seed:",
        "Randomize Grid with Seed",
        0,         // Default value
        LONG_MIN,  // Minimum value
        LONG_MAX,  // Maximum value
        this
    );

    if (seed != -1) // -1 is returned if the user cancels
    {
        RandomizeGrid(static_cast<int>(seed));
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

int MainWindow::CountLivingNeighbors(int row, int col)
{
    int livingNeighbors = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0) continue;
            int neighborRow = row + i;
            int neighborCol = col + j;
            if (neighborRow >= 0 && neighborRow < settings.gridSize &&
                neighborCol >= 0 && neighborCol < settings.gridSize)
            {
                if (gameBoard[neighborRow][neighborCol])
                {
                    ++livingNeighbors;
                }
            }
        }
    }
    return livingNeighbors;
}

void MainWindow::CalculateNextGeneration()
{
    // Ensure sandbox is the correct size to mirror gameBoard
    std::vector<std::vector<bool>> sandbox(settings.gridSize, std::vector<bool>(settings.gridSize, false));

    // Ensure neighborCounts is resized and initialized correctly
    neighborCounts.resize(settings.gridSize);
    for (int i = 0; i < settings.gridSize; ++i)
    {
        neighborCounts[i].resize(settings.gridSize, 0);
    }

    int newLivingCellsCount = 0;

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            // Count the living neighbors for the current cell
            int livingNeighbors = CountLivingNeighbors(row, col);

            // Store the neighbor count in the neighborCounts vector
            neighborCounts[row][col] = livingNeighbors;

            // Apply the rules of the Game of Life
            if (gameBoard[row][col])
            {
                // Rule 1: Any live cell with fewer than two or more than three live neighbors dies
                if (livingNeighbors < 2 || livingNeighbors > 3)
                {
                    sandbox[row][col] = false;
                }
                else
                {
                    // Rule 2: Any live cell with two or three live neighbors lives on to the next generation
                    sandbox[row][col] = true;
                    ++newLivingCellsCount;
                }
            }
            else
            {
                // Rule 3: Any dead cell with exactly three live neighbors becomes a live cell
                if (livingNeighbors == 3)
                {
                    sandbox[row][col] = true;
                    ++newLivingCellsCount;
                }
            }
        }
    }

    // Swap the sandbox with the gameBoard for the next generation
    gameBoard.swap(sandbox);

    // Update living cells count and generation count
    livingCellsCount = newLivingCellsCount;
    ++generationCount;

    // Update the status bar with the new counts
    UpdateStatusBar();

    // Pass the neighbor counts to the drawing panel and refresh it
    drawingPanel->SetNeighborCounts(neighborCounts);
    drawingPanel->Refresh();
}

void MainWindow::RandomizeGrid(int seed)
{
    srand(seed); // Seed the random number generator

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            // Generate a random number and decide if the cell should be alive
            gameBoard[row][col] = (rand() % 100 < 50); // 50% chance of being alive
        }
    }

    livingCellsCount = 0;
    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            if (gameBoard[row][col])
            {
                ++livingCellsCount;
            }
        }
    }

    generationCount = 0;
    UpdateStatusBar();
    drawingPanel->Refresh();
}


void MainWindow::OnPlay(wxCommandEvent& event)
{
    timer->Start(settings.interval);
}

void MainWindow::OnPause(wxCommandEvent& event)
{
    timer->Stop();
}

void MainWindow::OnNext(wxCommandEvent& event)
{
    CalculateNextGeneration();
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            gameBoard[row][col] = false;
        }
    }

    livingCellsCount = 0;
    generationCount = 0;

    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
    CalculateNextGeneration();
}

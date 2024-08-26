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
#include <wx/filedlg.h>
#include <wx/textfile.h>


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
EVT_MENU(ID_New, MainWindow::OnNew)
EVT_MENU(ID_Open, MainWindow::OnOpen)
EVT_MENU(ID_Save, MainWindow::OnSave)
EVT_MENU(ID_SaveAs, MainWindow::OnSaveAs)
EVT_MENU(ID_Exit, MainWindow::OnExit)
EVT_MENU(ID_Finite, MainWindow::OnFinite)
EVT_MENU(ID_Toroidal, MainWindow::OnToroidal)
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

    // File Menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_New, "&New\tCtrl-N", "Clear the grid and start a new game");
    fileMenu->Append(ID_Open, "&Open...\tCtrl-O", "Open an existing game board");
    fileMenu->Append(ID_Save, "&Save\tCtrl-S", "Save the current game board");
    fileMenu->Append(ID_SaveAs, "Save &As...\tCtrl-Shift-S", "Save the current game board under a new name");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_Exit, "E&xit", "Exit the application");

    menuBar->Append(fileMenu, "&File");

    // Options Menu
    optionsMenu = new wxMenu();
    optionsMenu->Append(ID_Settings, "&Settings...\tCtrl-S", "Configure settings");
    optionsMenu->Append(ID_Randomize, "&Randomize Grid", "Fill the grid with random cells");
    optionsMenu->Append(ID_RandomizeWithSeed, "&Randomize Grid with Seed...", "Fill the grid with random cells using a specific seed");
    menuBar->Append(optionsMenu, "&Options");

    // View Menu 
    wxMenu* viewMenu = new wxMenu();
    viewMenu->AppendCheckItem(ID_ShowNeighborCount, "Show Neighbor Count", "Show number of living neighbors");
    viewMenu->Check(ID_ShowNeighborCount, settings.showNeighborCount);  // Set initial check state

    // Add Finite and Toroidal options
    finiteItem = new wxMenuItem(viewMenu, ID_Finite, "Finite", "", wxITEM_CHECK);
    toroidalItem = new wxMenuItem(viewMenu, ID_Toroidal, "Toroidal", "", wxITEM_CHECK);

    viewMenu->Append(finiteItem);
    viewMenu->Append(toroidalItem);

    finiteItem->Check(!settings.isToroidal);
    toroidalItem->Check(settings.isToroidal);

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

            if (settings.isToroidal)
            {
                // Wrap around if out of bounds
                if (neighborRow < 0) neighborRow = settings.gridSize - 1;
                if (neighborRow >= settings.gridSize) neighborRow = 0;
                if (neighborCol < 0) neighborCol = settings.gridSize - 1;
                if (neighborCol >= settings.gridSize) neighborCol = 0;
            }
            else
            {
                // Skip out-of-bounds indices for finite universe
                if (neighborRow < 0 || neighborRow >= settings.gridSize ||
                    neighborCol < 0 || neighborCol >= settings.gridSize)
                {
                    continue;
                }
            }

            if (gameBoard[neighborRow][neighborCol])
            {
                ++livingNeighbors;
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

void MainWindow::OnNew(wxCommandEvent& event)
{
    InitializeGrid();  // Clear the game board
    currentFileName.clear();  // Clear the current file name
    drawingPanel->Refresh();  // Refresh the drawing panel
}


void MainWindow::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Game Board", "", "",
        "Cells files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxTextFile file(openFileDialog.GetPath());
    if (!file.Open())
        return;

    currentFileName = openFileDialog.GetPath();
    std::vector<wxString> fileContents;

    for (size_t i = 0; i < file.GetLineCount(); i++)
    {
        wxString line = file.GetLine(i);
        if (!line.StartsWith("!")) // Ignore comment lines
        {
            fileContents.push_back(line);
        }
    }

    int newGridSize = fileContents.size();
    settings.gridSize = newGridSize;
    InitializeGrid();

    for (int row = 0; row < newGridSize; ++row)
    {
        for (int col = 0; col < newGridSize; ++col)
        {
            if (col < fileContents[row].size() && fileContents[row][col] == '*')
            {
                gameBoard[row][col] = true;
            }
        }
    }

    drawingPanel->Refresh();
}

void MainWindow::OnSaveAs(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Save Game Board", "", "",
        "Cells files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    currentFileName = saveFileDialog.GetPath();
    SaveToFile(currentFileName);
}

void MainWindow::OnSave(wxCommandEvent& event)
{
    if (currentFileName.empty())
    {
        OnSaveAs(event);
    }
    else
    {
        SaveToFile(currentFileName);
    }
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainWindow::SaveToFile(const wxString& fileName)
{
    wxTextFile file;
    if (wxFile::Exists(fileName))
    {
        file.Open(fileName);
        file.Clear();
    }
    else
    {
        file.Create(fileName);
    }

    for (int row = 0; row < settings.gridSize; ++row)
    {
        wxString line;
        for (int col = 0; col < settings.gridSize; ++col)
        {
            line += gameBoard[row][col] ? '*' : '.';
        }
        file.AddLine(line);
    }

    file.Write();
    file.Close();
}
// In MainWindow.cpp

void MainWindow::OnFinite(wxCommandEvent& event)
{
    settings.isToroidal = false;
    finiteItem->Check(true);
    toroidalItem->Check(false);
    settings.Save();
    drawingPanel->Refresh();
}

void MainWindow::OnToroidal(wxCommandEvent& event)
{
    settings.isToroidal = true;
    finiteItem->Check(false);
    toroidalItem->Check(true);
    settings.Save();
    drawingPanel->Refresh();
}


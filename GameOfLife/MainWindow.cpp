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
EVT_TIMER(wxID_ANY, MainWindow::OnTimer)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxPoint(0, 0), wxSize(400, 400)),
    timer(new wxTimer(this, wxID_ANY))
{
    InitializeGrid();

    drawingPanel = new DrawingPanel(this, gameBoard);
    drawingPanel->SetSettings(&settings); // Pass settings to the drawing panel

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
    for (int i = 0; i < settings.gridSize; ++i)
    {
        gameBoard[i].resize(settings.gridSize, false);
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
    std::vector<std::vector<bool>> sandbox = gameBoard;
    sandbox.resize(settings.gridSize);
    for (int i = 0; i < settings.gridSize; ++i)
    {
        sandbox[i].resize(settings.gridSize, false);
    }

    int newLivingCellsCount = 0;

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            int livingNeighbors = CountLivingNeighbors(row, col);
            if (gameBoard[row][col])
            {
                if (livingNeighbors < 2 || livingNeighbors > 3)
                {
                    sandbox[row][col] = false;
                }
                else
                {
                    sandbox[row][col] = true;
                    ++newLivingCellsCount;
                }
            }
            else
            {
                if (livingNeighbors == 3)
                {
                    sandbox[row][col] = true;
                    ++newLivingCellsCount;
                }
            }
        }
    }

    gameBoard.swap(sandbox);

    livingCellsCount = newLivingCellsCount;
    ++generationCount;

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

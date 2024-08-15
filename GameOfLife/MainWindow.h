#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"
#include <vector>

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title);
    virtual ~MainWindow();

private:
    DrawingPanel* drawingPanel;
    wxBoxSizer* sizer;
    wxStatusBar* statusBar; // Status bar variable
    int gridSize = 15;
    int generationCount = 0; // Number of generations
    int livingCellsCount = 0; // Number of living cells
    std::vector<std::vector<bool>> gameBoard;

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar(); // Method to update the status bar

    wxDECLARE_EVENT_TABLE();
};

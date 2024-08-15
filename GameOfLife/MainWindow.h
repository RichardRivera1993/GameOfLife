#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"
#include <vector>


//IDs for the toolbar buttons
enum
{
    ID_Play = 10001,
    ID_Pause,
    ID_Next,
    ID_Clear
};
class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title);
    virtual ~MainWindow();

private:
    DrawingPanel* drawingPanel;
    wxBoxSizer* sizer;
    wxStatusBar* statusBar; // Status bar variable
    wxToolBar* toolBar; //Toolbar variable
    int gridSize = 15;
    int generationCount = 0; // Number of generations
    int livingCellsCount = 0; // Number of living cells
    std::vector<std::vector<bool>> gameBoard;

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar(); // Method to update the status bar
    int CountLivingNeighbors(int row, int col); // method to count living neighbors

    //handlers for toolbar buttons
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"

enum
{
    ID_Play = 10001,
    ID_Pause,
    ID_Next,
    ID_Clear,
    ID_Settings
};

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title);
    virtual ~MainWindow();

private:
    DrawingPanel* drawingPanel;
    wxBoxSizer* sizer;
    wxStatusBar* statusBar;
    wxToolBar* toolBar;
    wxMenuBar* menuBar;
    wxMenu* optionsMenu;

    wxTimer* timer;
    Settings settings;

    int generationCount = 0;
    int livingCellsCount = 0;
    std::vector<std::vector<bool>> gameBoard;

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar();
    int CountLivingNeighbors(int row, int col);
    void CalculateNextGeneration();

    // Event handlers for toolbar buttons and menu items
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);

    void OnTimer(wxTimerEvent& event);

    wxDECLARE_EVENT_TABLE();
};

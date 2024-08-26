#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include <vector>

enum
{
    ID_Play = 10001,
    ID_Pause,
    ID_Next,
    ID_Clear,
    ID_Settings,
    ID_ShowNeighborCount,
    ID_Randomize,
    ID_RandomizeWithSeed,
    ID_New,             // New ID for New option
    ID_Open,            // New ID for Open option
    ID_Save,            // New ID for Save option
    ID_SaveAs,          // New ID for Save As option
    ID_Exit,             // New ID for Exit option
    ID_Finite,
    ID_Toroidal,
    ID_ResetSettings
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
    wxMenuItem* finiteItem;
    wxMenuItem* toroidalItem;

    wxTimer* timer;
    Settings settings;
    wxString currentFileName; //store the current file name

    int generationCount = 0;
    int livingCellsCount = 0;
    std::vector<std::vector<bool>> gameBoard;

    std::vector<std::vector<int>> neighborCounts;

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar();
    int CountLivingNeighbors(int row, int col);
    void CalculateNextGeneration();
    void RandomizeGrid(int seed);

    // Event handlers for toolbar buttons and menu items
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnToggleShowNeighborCount(wxCommandEvent& event);
    void OnRandomize(wxCommandEvent& event);
    void OnRandomizeWithSeed(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnFinite(wxCommandEvent& event);
    void OnToroidal(wxCommandEvent& event);
    void SaveToFile(const wxString& fileName);

    void OnResetSettings(wxCommandEvent& event); // new handler event

    void OnTimer(wxTimerEvent& event);

    wxDECLARE_EVENT_TABLE();
};

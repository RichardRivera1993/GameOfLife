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
    ID_New,
    ID_Open,
    ID_Save,
    ID_SaveAs,
    ID_Exit,
    ID_Finite,
    ID_Toroidal,
    ID_ResetSettings,
    ID_Import,
    ID_ShowGrid,
    ID_ShowThickGrid,
    ID_ShowHUD
};

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title);
    virtual ~MainWindow();
    int GetGenerationCount() const;
    int GetLivingCellsCount() const;
private:
    DrawingPanel* drawingPanel;
    wxBoxSizer* sizer;
    wxStatusBar* statusBar;
    wxToolBar* toolBar;
    wxMenuBar* menuBar;
    wxMenu* optionsMenu;

    wxTimer* timer;
    Settings settings;
    wxString currentFileName; // Store the current file name

    int generationCount = 0;
    int livingCellsCount = 0;
    std::vector<std::vector<bool>> gameBoard;

    std::vector<std::vector<int>> neighborCounts;

    wxMenuItem* finiteItem;
    wxMenuItem* toroidalItem;

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar();
    int CountLivingNeighbors(int row, int col);
    void CalculateNextGeneration();
    void RandomizeGrid(int seed);
    void OnTimer(wxTimerEvent& event);

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
    void SaveToFile(const wxString& fileName);

    // View menu handlers
    void OnFinite(wxCommandEvent& event);
    void OnToroidal(wxCommandEvent& event);
    void OnResetSettings(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void OnToggleShowGrid(wxCommandEvent& event);
    void OnToggleShowThickGrid(wxCommandEvent& event);
    void OnToggleShowHUD(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

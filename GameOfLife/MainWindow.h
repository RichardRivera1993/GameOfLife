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
    wxBoxSizer* sizer; // Add a wxBoxSizer pointer
    int gridSize = 15; // Moved grid size to MainWindow
    std::vector<std::vector<bool>> gameBoard; // Game board data structure

    void InitializeGrid(); // Method to initialize the grid
    void OnSizeChange(wxSizeEvent& event); // Declare OnSizeChange method

    wxDECLARE_EVENT_TABLE();
};

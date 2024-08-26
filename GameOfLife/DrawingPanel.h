#pragma once

#include "wx/wx.h"
#include "Settings.h"
#include <vector>

class MainWindow;  // Forward declaration of MainWindow class

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent, MainWindow* mainWin, std::vector<std::vector<bool>>& gameBoard);
    virtual ~DrawingPanel();
    void SetSize(const wxSize& size);
    void SetGridSize(int size);
    void SetSettings(Settings* settings); // Setter for settings pointer
    void SetNeighborCounts(const std::vector<std::vector<int>>& counts); // method to set neighbor counts

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void DrawHUD(wxGraphicsContext* context);

    std::vector<std::vector<bool>>& gameBoard;
    Settings* settings;
    MainWindow* mainWin;  // Pointer to MainWindow

    std::vector<std::vector<int>> neighborCounts;

    wxDECLARE_EVENT_TABLE();
};

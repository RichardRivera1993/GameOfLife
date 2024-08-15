#pragma once

#include "wx/wx.h"
#include <vector>

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard);
    virtual ~DrawingPanel();
    void SetSize(const wxSize& size);
    void SetGridSize(int size);

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseUp(wxMouseEvent& event); // Mouse event handler

    std::vector<std::vector<bool>>& gameBoard; // Reference to the game board
    int gridSize; // Grid size

    wxDECLARE_EVENT_TABLE();
};

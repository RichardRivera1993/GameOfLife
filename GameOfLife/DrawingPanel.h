#pragma once

#include "wx/wx.h"
#include "Settings.h"
#include <vector>

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard);
    virtual ~DrawingPanel();
    void SetSize(const wxSize& size);
    void SetGridSize(int size);
    void SetSettings(Settings* settings); // Setter for settings pointer

private:
    void OnPaint(wxPaintEvent& event);
    void OnMouseUp(wxMouseEvent& event);

    std::vector<std::vector<bool>>& gameBoard;
    Settings* settings;

    wxDECLARE_EVENT_TABLE();
};

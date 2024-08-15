#pragma once

#include "wx/wx.h"

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent);
    virtual ~DrawingPanel();
    void SetSize(const wxSize& size); // Add SetSize method

private:
    void OnPaint(wxPaintEvent& event);
    int gridSize = 15; // Default grid size
};

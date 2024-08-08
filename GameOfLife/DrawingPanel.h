#pragma once

#include "wx/wx.h"

class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent);
    virtual ~DrawingPanel();

private:
    void OnPaint(wxPaintEvent& event);
};

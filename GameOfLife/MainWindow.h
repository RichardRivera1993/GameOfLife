#pragma once

#include "wx/wx.h"
#include "DrawingPanel.h"
class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title);
    virtual ~MainWindow();


private:
    DrawingPanel* drawingPanel;
    };
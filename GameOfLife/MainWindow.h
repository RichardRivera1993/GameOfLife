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
    wxBoxSizer* sizer; // Add a wxBoxSizer pointer
    void OnSizeChange(wxSizeEvent& event); // Declare OnSizeChange method
};

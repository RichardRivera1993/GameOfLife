#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

DrawingPanel::~DrawingPanel()
{
    // Cleanup code, if needed
}

void DrawingPanel::SetSize(const wxSize& size)
{
    wxPanel::SetSize(size); // Set the size of the panel
    Refresh(); // Refresh to trigger a repaint with the new size
}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this); // Double buffering to prevent flickering
    dc.Clear(); // Clear the entire drawing surface

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) return;

    context->SetPen(*wxBLACK);
    context->SetBrush(*wxWHITE);

    // Get the size of the DrawingPanel
    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);

    // Calculate cell size based on panel size and grid size
    int cellWidth = panelWidth / gridSize;
    int cellHeight = panelHeight / gridSize;

    // Draw the grid
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            int x = col * cellWidth;
            int y = row * cellHeight;
            context->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    delete context;
}

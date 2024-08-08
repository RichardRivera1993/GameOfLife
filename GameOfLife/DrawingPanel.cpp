#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

void DrawingPanel::OnSize(wxSizeEvent& event)
{
    Refresh(); // Force the panel to redraw itself
    event.Skip(); // Continue processing the event
}


DrawingPanel::DrawingPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    this->Bind(wxEVT_SIZE, &DrawingPanel::OnSize, this); // Bind the resize event
}

DrawingPanel::~DrawingPanel()
{
    // Cleanup code, if needed
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



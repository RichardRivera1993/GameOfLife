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

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) return;

    context->SetPen(*wxBLACK);
    context->SetBrush(*wxWHITE);

    int cellSize = 10; // Each cell is 10x10 pixels

    // Loop through each row
    for (int row = 0; row < gridSize; row++)
    {
        // Loop through each column
        for (int col = 0; col < gridSize; col++)
        {
            int x = col * cellSize;
            int y = row * cellSize;
            context->DrawRectangle(x, y, cellSize, cellSize);
        }
    }

    delete context;
}

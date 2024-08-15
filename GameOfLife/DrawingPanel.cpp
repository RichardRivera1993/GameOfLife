#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"


wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard)
    : wxPanel(parent, wxID_ANY), gameBoard(gameBoard), gridSize(15) // Initialize game board reference
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel()
{
    // Cleanup code, if needed
}

void DrawingPanel::SetSize(const wxSize& size)
{
    wxPanel::SetSize(size);
    Refresh();
}

void DrawingPanel::SetGridSize(int size)
{
    gridSize = size;
    Refresh();
}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) return;

    context->SetPen(*wxBLACK);

    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);

    int cellWidth = panelWidth / gridSize;
    int cellHeight = panelHeight / gridSize;

    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            int x = col * cellWidth;
            int y = row * cellHeight;

            // Check the corresponding cell in the game board
            if (gameBoard[row][col])
            {
                context->SetBrush(*wxLIGHT_GREY); // Living cell
            }
            else
            {
                context->SetBrush(*wxWHITE); // Dead cell
            }

            // Draw the rectangle for this cell
            context->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    delete context;
}


void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{
    int x = event.GetX();
    int y = event.GetY();

    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);

    int cellWidth = panelWidth / gridSize;
    int cellHeight = panelHeight / gridSize;

    int col = x / cellWidth;
    int row = y / cellHeight;

    if (row >= 0 && row < gridSize && col >= 0 && col < gridSize) {
        gameBoard[row][col] = !gameBoard[row][col]; // Toggle the cell's state
        Refresh(); // Trigger a repaint to reflect the change
    }
}

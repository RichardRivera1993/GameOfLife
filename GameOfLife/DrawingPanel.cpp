#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard)
    : wxPanel(parent, wxID_ANY), gameBoard(gameBoard), settings(nullptr)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel()
{
}

void DrawingPanel::SetSettings(Settings* settings)
{
    this->settings = settings;
    Refresh();  // Redraw the panel with the new settings
}

void DrawingPanel::SetNeighborCounts(const std::vector<std::vector<int>>& counts)
{
    neighborCounts = counts;
    Refresh();
}

void DrawingPanel::SetSize(const wxSize& size)
{
    wxPanel::SetSize(size);
    Refresh();
}

void DrawingPanel::SetGridSize(int size)
{
    settings->gridSize = size;
    Refresh();
}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* context = wxGraphicsContext::Create(dc);
    if (!context) return;

    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);

    int cellWidth = panelWidth / settings->gridSize;
    int cellHeight = panelHeight / settings->gridSize;

    // Set grid visibility
    if (settings->showGrid)
    {
        context->SetPen(wxPen(wxColor(0, 0, 0, 64)));  // Light gray grid
    }
    else
    {
        context->SetPen(wxPen(wxTransparentColor));
    }

    // Draw the grid
    for (int row = 0; row <= settings->gridSize; ++row)
    {
        int y = row * cellHeight;
        context->StrokeLine(0, y, panelWidth, y);
    }

    for (int col = 0; col <= settings->gridSize; ++col)
    {
        int x = col * cellWidth;
        context->StrokeLine(x, 0, x, panelHeight);
    }

    // Draw thicker 10x10 grid lines
    if (settings->showThickGrid)
    {
        dc.SetPen(wxPen(*wxBLACK, 2));  // Thicker black lines

        int numThickLines = settings->gridSize / 10;
        for (int i = 1; i <= numThickLines; ++i)
        {
            int x = i * 10 * cellWidth;
            int y = i * 10 * cellHeight;

            dc.DrawLine(x, 0, x, panelHeight);  // Vertical line
            dc.DrawLine(0, y, panelWidth, y);  // Horizontal line
        }
    }

    // Draw the cells and neighbor counts (existing logic)
    for (int row = 0; row < settings->gridSize; ++row)
    {
        for (int col = 0; col < settings->gridSize; ++col)
        {
            int x = col * cellWidth;
            int y = row * cellHeight;

            if (gameBoard[row][col])
            {
                context->SetBrush(wxBrush(settings->GetLivingCellColor()));
            }
            else
            {
                context->SetBrush(wxBrush(settings->GetDeadCellColor()));
            }

            context->DrawRectangle(x, y, cellWidth, cellHeight);

            if (settings->showNeighborCount && neighborCounts[row][col] > 0)
            {
                context->SetFont(wxFontInfo(16), *wxRED);
                wxString text = wxString::Format("%d", neighborCounts[row][col]);
                double textWidth, textHeight;
                context->GetTextExtent(text, &textWidth, &textHeight);
                double textX = x + (cellWidth - textWidth) / 2;
                double textY = y + (cellHeight - textHeight) / 2;
                context->DrawText(text, textX, textY);
            }
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

    int cellWidth = panelWidth / settings->gridSize;
    int cellHeight = panelHeight / settings->gridSize;

    int col = x / cellWidth;
    int row = y / cellHeight;

    if (row >= 0 && row < settings->gridSize && col >= 0 && col < settings->gridSize) {
        gameBoard[row][col] = !gameBoard[row][col];
        Refresh();
    }
}

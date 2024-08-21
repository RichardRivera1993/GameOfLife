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

    context->SetPen(*wxBLACK);

    int panelWidth, panelHeight;
    GetClientSize(&panelWidth, &panelHeight);

    int cellWidth = panelWidth / settings->gridSize;
    int cellHeight = panelHeight / settings->gridSize;

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

            // Ensure neighborCounts is properly initialized before accessing it
            if (settings->showNeighborCount && row < neighborCounts.size() && col < neighborCounts[row].size() && neighborCounts[row][col] > 0)
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

#include "DrawingPanel.h"
#include "MainWindow.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, MainWindow* mainWin, std::vector<std::vector<bool>>& gameBoard)
    : wxPanel(parent, wxID_ANY), gameBoard(gameBoard), settings(nullptr), mainWin(mainWin)
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

    // Calculate the cell dimensions, allowing for rectangular cells
    double cellWidth = static_cast<double>(panelWidth) / settings->gridSize;
    double cellHeight = static_cast<double>(panelHeight) / settings->gridSize;

    for (int row = 0; row < settings->gridSize; ++row)
    {
        for (int col = 0; col < settings->gridSize; ++col)
        {
            double x = col * cellWidth;
            double y = row * cellHeight;

            // Set the brush color based on whether the cell is alive or dead
            if (gameBoard[row][col])
            {
                context->SetBrush(wxBrush(settings->GetLivingCellColor()));
            }
            else
            {
                context->SetBrush(wxBrush(settings->GetDeadCellColor()));
            }

            context->DrawRectangle(x, y, cellWidth, cellHeight);

            // Draw neighbor count if enabled
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

    // Only draw the grid if the setting is enabled
    if (settings->showGrid)
    
    {
        // Set pen color for the regular grid lines
        context->SetPen(wxPen(wxColor(0, 0, 0, 64)));  // Light gray for regular grid
    }
    else
    {
        context->SetPen(*wxTRANSPARENT_PEN);
    }
        for (int i = 1; i < settings->gridSize; ++i)
        {
            double linePosX = i * cellWidth;
            double linePosY = i * cellHeight;
            context->StrokeLine(linePosX, 0, linePosX, panelHeight); // Vertical line
            context->StrokeLine(0, linePosY, panelWidth, linePosY);  // Horizontal line
        }

        // Draw thicker lines every 10 cells if the setting is enabled
        if (settings->showThickGrid)
        {
            context->SetPen(wxPen(wxColor(0, 0, 0), 2));  // Black for thicker 10x10 grid
            int thickLineCount = settings->gridSize / 10;
            for (int i = 1; i <= thickLineCount; ++i)
            {
                double linePosX = i * 10 * cellWidth;
                double linePosY = i * 10 * cellHeight;
                context->StrokeLine(linePosX, 0, linePosX, panelHeight); // Vertical thick line
                context->StrokeLine(0, linePosY, panelWidth, linePosY);  // Horizontal thick line
            }
        }
    

    // HUD Drawing
    DrawHUD(context);

    delete context;
}



void DrawingPanel::DrawHUD(wxGraphicsContext* context)
{
    if (settings->showHUD)
    {
        wxString hudText = wxString::Format("Generations: %d\nLiving Cells: %d\nBoundary: %s\nUniverse Size: %d",
            mainWin->GetGenerationCount(),
            mainWin->GetLivingCellsCount(),
            settings->isToroidal ? "Toroidal" : "Finite",
            settings->gridSize);

        double textWidth, textHeight;
        context->SetFont(wxFontInfo(16), *wxRED);
        context->GetTextExtent(hudText, &textWidth, &textHeight);
        context->DrawText(hudText, 10, GetSize().GetHeight() - textHeight - 10);  // Draw HUD in bottom-left corner
    }
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

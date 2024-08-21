#pragma once
#include "wx/wx.h"

struct Settings
{
    // Color for living cells
    unsigned int livingCellRed = 128;
    unsigned int livingCellGreen = 128;
    unsigned int livingCellBlue = 128;
    unsigned int livingCellAlpha = 255;

    // Color for dead cells
    unsigned int deadCellRed = 255;
    unsigned int deadCellGreen = 255;
    unsigned int deadCellBlue = 255;
    unsigned int deadCellAlpha = 255;

    // Grid size
    int gridSize = 15;

    // Timer interval in milliseconds
    int interval = 50;

    // Method to get the living cell color as wxColor
    wxColor GetLivingCellColor() const
    {
        return wxColor(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
    }

    // Method to get the dead cell color as wxColor
    wxColor GetDeadCellColor() const
    {
        return wxColor(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
    }

    // Method to set the living cell color from wxColor
    void SetLivingCellColor(const wxColor& color)
    {
        livingCellRed = color.Red();
        livingCellGreen = color.Green();
        livingCellBlue = color.Blue();
        livingCellAlpha = color.Alpha();
    }

    // Method to set the dead cell color from wxColor
    void SetDeadCellColor(const wxColor& color)
    {
        deadCellRed = color.Red();
        deadCellGreen = color.Green();
        deadCellBlue = color.Blue();
        deadCellAlpha = color.Alpha();
    }
};

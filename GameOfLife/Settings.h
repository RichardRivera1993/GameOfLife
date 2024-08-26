#pragma once

#include "wx/wx.h"
#include <fstream>

struct Settings
{
    // Existing color and grid size settings
    unsigned int livingCellRed = 128;
    unsigned int livingCellGreen = 128;
    unsigned int livingCellBlue = 128;
    unsigned int livingCellAlpha = 255;

    unsigned int deadCellRed = 255;
    unsigned int deadCellGreen = 255;
    unsigned int deadCellBlue = 255;
    unsigned int deadCellAlpha = 255;

    int gridSize = 15;
    int interval = 50;

    // New settings for boundary type, grid, and HUD
    bool isToroidal = false;
    bool showGrid = true;
    bool showThickGrid = false;
    bool showHUD = false;
    bool showNeighborCount = false;

    wxColor GetLivingCellColor() const
    {
        return wxColor(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
    }

    wxColor GetDeadCellColor() const
    {
        return wxColor(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
    }

    void SetLivingCellColor(const wxColor& color)
    {
        livingCellRed = color.Red();
        livingCellGreen = color.Green();
        livingCellBlue = color.Blue();
        livingCellAlpha = color.Alpha();
    }

    void SetDeadCellColor(const wxColor& color)
    {
        deadCellRed = color.Red();
        deadCellGreen = color.Green();
        deadCellBlue = color.Blue();
        deadCellAlpha = color.Alpha();
    }

    // Method to reset all settings to their default values
    void ResetToDefault()
    {
        livingCellRed = 128;
        livingCellGreen = 128;
        livingCellBlue = 128;
        livingCellAlpha = 255;

        deadCellRed = 255;
        deadCellGreen = 255;
        deadCellBlue = 255;
        deadCellAlpha = 255;

        gridSize = 15;
        interval = 50;

        isToroidal = false;
        showGrid = true;
        showThickGrid = false;
        showHUD = false;
        showNeighborCount = false;
    }

    // Method to load settings from file
    void Load()
    {
        std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
        if (file.is_open())
        {
            file.read((char*)this, sizeof(Settings));
            file.close();
        }
    }

    // Method to save settings to file
    void Save() const
    {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open())
        {
            file.write((char*)this, sizeof(Settings));
            file.close();
        }
    }
};

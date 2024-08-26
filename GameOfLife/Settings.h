#pragma once

#include "wx/wx.h"
#include <fstream>  // Include for file operations

struct Settings
{
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
    bool showNeighborCount = false;  // New option to show neighbor count

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

    // Method to load settings from a file
    void Load()
    {
        std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
        if (file.is_open())
        {
            file.read((char*)this, sizeof(Settings));
            file.close();
        }
    }

    // Method to save settings to a file
    void Save() const
    {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open())
        {
            file.write((char*)this, sizeof(Settings));
            file.close();
        }
    }
    bool isToroidal = false; //Defaults to finite
};

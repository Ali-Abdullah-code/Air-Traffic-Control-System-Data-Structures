#ifndef RADAR_H
#define RADAR_H

#include <string>
#include <iostream>

using namespace std;

const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 20;

// ========== COLOR CODES FOR CONSOLE ==========
class Color
{
public:
    // Text Colors
    static const string RESET;
    static const string BLACK;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;

    // Background Colors
    static const string BG_BLACK;
    static const string BG_RED;
    static const string BG_GREEN;
    static const string BG_YELLOW;
    static const string BG_BLUE;
    static const string BG_MAGENTA;
    static const string BG_CYAN;
    static const string BG_WHITE;

    // Text Styles
    static const string BOLD;
    static const string DIM;
    static const string ITALIC;
    static const string UNDERLINE;
};

// ========== RADAR CLASS ==========
class Radar
{
private:
    char grid[GRID_HEIGHT][GRID_WIDTH];
    int height;
    int width;

public:
    // Constructor & Destructor
    Radar();
    ~Radar();

    // Initialize grid
    void initialize();

    // Place objects on grid
    void placeAirport(int x, int y, string airportName, bool silent = false);
    void placePlane(int x, int y, string flightID, bool silent = false);
    void placeWaypoint(int x, int y, string waypointID, bool silent = false);

    // Remove objects from grid
    void clearCell(int x, int y);
    void clearPlane(int x, int y);

    // Grid management
    bool isValidPosition(int x, int y);
    char getCell(int x, int y);
    void setCell(int x, int y, char symbol);

    // Display
    void displayRadar();
    void displayWithBorder();
    void displayLegend();
    void displayObjectInfo();
    void clearScreen();
    void refreshDisplay();

    // Get grid info
    int getWidth();
    int getHeight();
    void printGridInfo();
};

#endif // RADAR_H
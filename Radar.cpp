#include "Radar.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ========== COLOR DEFINITIONS ==========
const string Color::RESET = "\033[0m";
const string Color::BLACK = "\033[30m";
const string Color::RED = "\033[31m";
const string Color::GREEN = "\033[32m";
const string Color::YELLOW = "\033[33m";
const string Color::BLUE = "\033[34m";
const string Color::MAGENTA = "\033[35m";
const string Color::CYAN = "\033[36m";
const string Color::WHITE = "\033[37m";

const string Color::BG_BLACK = "\033[40m";
const string Color::BG_RED = "\033[41m";
const string Color::BG_GREEN = "\033[42m";
const string Color::BG_YELLOW = "\033[43m";
const string Color::BG_BLUE = "\033[44m";
const string Color::BG_MAGENTA = "\033[45m";
const string Color::BG_CYAN = "\033[46m";
const string Color::BG_WHITE = "\033[47m";

const string Color::BOLD = "\033[1m";
const string Color::DIM = "\033[2m";
const string Color::ITALIC = "\033[3m";
const string Color::UNDERLINE = "\033[4m";

// ========== CONSTRUCTOR ==========
Radar::Radar()
{
    height = GRID_HEIGHT;
    width = GRID_WIDTH;
    initialize();
}

// ========== DESTRUCTOR ==========
Radar::~Radar()
{
    // Nothing to delete
}

// ========== INITIALIZE GRID ==========
void Radar::initialize()
{
    // Fill all cells with empty space
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            grid[i][j] = '.';
        }
    }
}

// ========== PLACE AIRPORT ==========
void Radar::placeAirport(int x, int y, string airportName, bool silent)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = 'A';
        if (!silent)
        {
            cout << Color::GREEN << "[INFO]" << Color::RESET
                 << " Airport '" << airportName << "' placed at ("
                 << x << ", " << y << ")" << endl;
        }
    }
    else if (!silent)
    {
        cout << Color::RED << "[ERROR]" << Color::RESET
             << " Invalid position (" << x << ", " << y << ")" << endl;
    }
}

// ========== PLACE PLANE ==========
void Radar::placePlane(int x, int y, string flightID, bool silent)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = 'P';
        if (!silent)
        {
            cout << Color::CYAN << "[INFO]" << Color::RESET
                 << " Plane '" << flightID << "' placed at ("
                 << x << ", " << y << ")" << endl;
        }
    }
    else if (!silent)
    {
        cout << Color::RED << "[ERROR]" << Color::RESET
             << " Invalid position (" << x << ", " << y << ")" << endl;
    }
}

// ========== PLACE WAYPOINT ==========
void Radar::placeWaypoint(int x, int y, string waypointID, bool silent)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = 'W';
        if (!silent)
        {
            cout << Color::YELLOW << "[INFO]" << Color::RESET
                 << " Waypoint '" << waypointID << "' placed at ("
                 << x << ", " << y << ")" << endl;
        }
    }
    else if (!silent)
    {
        cout << Color::RED << "[ERROR]" << Color::RESET
             << " Invalid position (" << x << ", " << y << ")" << endl;
    }
}

// ========== CLEAR CELL ==========
void Radar::clearCell(int x, int y)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = '.';
    }
}

// ========== CLEAR PLANE ==========
void Radar::clearPlane(int x, int y)
{
    if (isValidPosition(x, y) && grid[y][x] == 'P')
    {
        grid[y][x] = '.';
        cout << Color::CYAN << "[INFO]" << Color::RESET
             << " Plane removed from (" << x << ", " << y << ")" << endl;
    }
}

// ========== CHECK VALID POSITION ==========
bool Radar::isValidPosition(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

// ========== GET CELL ==========
char Radar::getCell(int x, int y)
{
    if (isValidPosition(x, y))
    {
        return grid[y][x];
    }
    return '?';
}

// ========== SET CELL ==========
void Radar::setCell(int x, int y, char symbol)
{
    if (isValidPosition(x, y))
    {
        grid[y][x] = symbol;
    }
}

// ========== CLEAR SCREEN ==========
void Radar::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ========== DISPLAY RADAR WITH BORDER ==========
void Radar::displayWithBorder()
{
    cout << Color::BOLD << Color::CYAN;
    cout << "\n+=============================================================+\n";
    cout << "|           " << Color::YELLOW << "SkyNet ATC - RADAR DISPLAY" << Color::CYAN << "           |\n";
    cout << "+=============================================================+\n";
    cout << Color::RESET;

    // Top border with coordinates
    cout << Color::BOLD << Color::BLUE << "  ";
    for (int j = 0; j < width; j++)
    {
        cout << (j % 10);
    }
    cout << Color::RESET << endl;

    // Grid with left border
    for (int i = 0; i < height; i++)
    {
        cout << Color::BOLD << Color::BLUE << setw(2) << i << Color::RESET;

        for (int j = 0; j < width; j++)
        {
            char cell = grid[i][j];

            // Color code for different symbols
            if (cell == 'A')
            {
                cout << Color::GREEN << Color::BOLD << cell << Color::RESET;
            }
            else if (cell == 'P')
            {
                cout << Color::RED << Color::BOLD << cell << Color::RESET;
            }
            else if (cell == 'W')
            {
                cout << Color::YELLOW << Color::BOLD << cell << Color::RESET;
            }
            else
            {
                cout << Color::DIM << cell << Color::RESET;
            }
        }
        cout << endl;
    }

    cout << Color::BLUE << Color::BOLD << "  ";
    for (int j = 0; j < width; j++)
    {
        cout << (j % 10);
    }
    cout << Color::RESET << endl;
}

// ========== DISPLAY LEGEND ==========
void Radar::displayLegend()
{
    cout << Color::BOLD << Color::CYAN << "\n+---------- LEGEND ----------+\n"
         << Color::RESET;

    cout << "  " << Color::GREEN << Color::BOLD << "A" << Color::RESET
         << " = Airport     "
         << Color::RED << Color::BOLD << "P" << Color::RESET
         << " = Plane        "
         << Color::YELLOW << Color::BOLD << "W" << Color::RESET
         << " = Waypoint\n";

    cout << "  " << Color::DIM << "." << Color::RESET
         << " = Empty Sky\n";

    cout << Color::BOLD << Color::CYAN << "+----------------------------+\n"
         << Color::RESET;
}

// ========== DISPLAY OBJECT INFORMATION ==========
void Radar::displayObjectInfo()
{
    cout << Color::BOLD << Color::MAGENTA
         << "+------------ OBJECTS ON RADAR -----------+\n"
         << Color::RESET;

    bool hasObjects = false;

    // Display all airports
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] == 'A')
            {
                hasObjects = true;
                cout << "  " << Color::GREEN << Color::BOLD << "[A]" << Color::RESET
                     << " AIRPORT at Position (" << j << ", " << i << ")\n";
            }
        }
    }

    // Display all planes
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] == 'P')
            {
                hasObjects = true;
                cout << "  " << Color::RED << Color::BOLD << "[P]" << Color::RESET
                     << " PLANE at Position (" << j << ", " << i << ")\n";
            }
        }
    }

    // Display all waypoints
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] == 'W')
            {
                hasObjects = true;
                cout << "  " << Color::YELLOW << Color::BOLD << "[W]" << Color::RESET
                     << " WAYPOINT at Position (" << j << ", " << i << ")\n";
            }
        }
    }

    if (!hasObjects)
    {
        cout << "  " << Color::DIM << "(No objects on radar)" << Color::RESET << "\n";
    }

    cout << Color::BOLD << Color::MAGENTA
         << "+----------------------------------------+\n"
         << Color::RESET;
}

// ========== DISPLAY RADAR (COMPLETE VERSION) ==========
void Radar::displayRadar()
{
    displayWithBorder();
    displayObjectInfo();
    displayLegend();
}

// ========== REFRESH DISPLAY ==========
void Radar::refreshDisplay()
{
    clearScreen();
    displayRadar();
}

// ========== GET WIDTH ==========
int Radar::getWidth()
{
    return width;
}

// ========== GET HEIGHT ==========
int Radar::getHeight()
{
    return height;
}

// ========== PRINT GRID INFO ==========
void Radar::printGridInfo()
{
    cout << Color::BOLD << Color::CYAN
         << "\n+------------ GRID INFO -----------+\n"
         << Color::RESET;
    cout << "  Grid Size: " << width << " x " << height << " cells\n";
    cout << "  Total Cells: " << (width * height) << "\n";

    int emptyCount = 0, objectCount = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (grid[i][j] == '.')
                emptyCount++;
            else
                objectCount++;
        }
    }

    cout << "  Objects: " << objectCount << "  |  Empty: " << emptyCount << "\n";
    cout << Color::BOLD << Color::CYAN
         << "+----------------------------------+\n"
         << Color::RESET;
}
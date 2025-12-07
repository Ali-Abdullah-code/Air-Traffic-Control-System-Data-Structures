#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include "Graph.h"
#include "Radar.h"
#include "FlightManager.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "AVLTree.h"

using namespace std;

// ========== GLOBAL OBJECTS ==========
Graph airspace;
Radar radar;
FlightManager flightMgr;
HashTable registry;
MinHeap landingQueue;
AVLTree flightLog;

// ========== FUNCTION PROTOTYPES ==========
void clearScreen();
void displayWelcome();
void displayMainMenu();
void initializeAirspace();
void displayAirspaceVisualizer();
void flightControlMenu();
void searchAndReportingMenu();
void systemMenu();
void displayFooter();
void updateRadarWithFlights();

// ========== WELCOME SCREEN ==========
void displayWelcome()
{
    clearScreen();
    cout << Color::BOLD << Color::CYAN;
    cout << "\n+==============================================================+\n";
    cout << "|                                                              |\n";
    cout << "|              " << Color::YELLOW << "WELCOME TO SKYNET ATC SIMULATOR" << Color::CYAN << "                 |\n";
    cout << "|                                                              |\n";
    cout << "|           Air Traffic Control Management System              |\n";
    cout << "|                                                              |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;

    cout << Color::GREEN << "Initializing systems...\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Graph initialized\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Radar system online\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Flight Manager ready\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Aircraft registry loaded\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Landing queue ready\n"
         << Color::RESET;
    cout << Color::GREEN << "[OK] Flight logs system ready\n\n"
         << Color::RESET;

    cout << Color::YELLOW << "Press ENTER to continue..." << Color::RESET;
    cin.ignore();
}

// ========== CLEAR SCREEN ==========
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ========== DISPLAY MAIN MENU ==========
void displayMainMenu()
{
    clearScreen();

    cout << Color::BOLD << Color::CYAN;
    cout << "\n+==============================================================+\n";
    cout << "|                    " << Color::YELLOW << "SKYNET ATC MAIN MENU" << Color::CYAN << "                      |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;

    cout << Color::BOLD << "  +-----------------------------------------------+\n";
    cout << "  | " << Color::RESET << Color::GREEN << "1. Airspace Visualizer" << Color::RESET << Color::BOLD << "                        |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "2. Flight Control" << Color::RESET << Color::BOLD << "                             |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "3. Search & Reporting" << Color::RESET << Color::BOLD << "                         |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "4. System Management" << Color::RESET << Color::BOLD << "                          |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "5. Aircraft Registry" << Color::RESET << Color::BOLD << "                          |\n";
    cout << "  | " << Color::RESET << Color::RED << "6. Exit Program" << Color::RESET << Color::BOLD << "                               |\n";
    cout << "  +-----------------------------------------------+\n";
    cout << Color::RESET << endl;

    cout << Color::CYAN << "Enter your choice (1-6): " << Color::RESET;
}

// ========== INITIALIZE AIRSPACE ==========
void initializeAirspace()
{
    cout << Color::BOLD << Color::YELLOW << "\n[SYSTEM] Initializing Airspace...\n"
         << Color::RESET;

    // Add airports with grid coordinates (0-29, 0-19)
    airspace.addNode(1, "JFK", 5, 2, AIRPORT);
    airspace.addNode(2, "LHR", 25, 5, AIRPORT);
    airspace.addNode(3, "CDG", 15, 8, AIRPORT);
    airspace.addNode(4, "DXB", 28, 12, AIRPORT);

    // Add waypoints with grid coordinates
    airspace.addNode(5, "W1", 10, 3, WAYPOINT);
    airspace.addNode(6, "W2", 20, 7, WAYPOINT);
    airspace.addNode(7, "W3", 5, 10, WAYPOINT);
    airspace.addNode(8, "W4", 23, 10, WAYPOINT);

    // Add edges (flight corridors) with integer weights
    airspace.addEdge(1, 5, 50);  // JFK to W1
    airspace.addEdge(5, 2, 100); // W1 to LHR
    airspace.addEdge(1, 6, 80);  // JFK to W2
    airspace.addEdge(6, 3, 75);  // W2 to CDG
    airspace.addEdge(2, 6, 90);  // LHR to W2
    airspace.addEdge(6, 4, 120); // W2 to DXB
    airspace.addEdge(3, 7, 110); // CDG to W3
    airspace.addEdge(7, 4, 130); // W3 to DXB
    airspace.addEdge(3, 8, 100); // CDG to W4
    airspace.addEdge(8, 4, 85);  // W4 to DXB

    // Set graph reference for flight manager
    flightMgr.setGraphReference(&airspace);

    // Set registry reference for flight manager
    flightMgr.setRegistryReference(&registry);

    cout << Color::GREEN << "[SUCCESS] Airspace initialized with 4 airports and 4 waypoints\n"
         << Color::RESET;
    cout << Color::GREEN << "[SUCCESS] Flight corridors established\n"
         << Color::RESET << endl;
}

// ========== UPDATE RADAR WITH FLIGHTS ==========
void updateRadarWithFlights()
{
    // Clear radar
    radar.initialize();

    // Place airports
    for (int i = 1; i <= airspace.getTotalNodes(); i++)
    {
        Node *node = airspace.getNode(i);
        if (node != NULL && node->type == AIRPORT)
        {
            radar.placeAirport(node->x, node->y, node->name, true);
        }
    }

    // Place waypoints
    for (int i = 1; i <= airspace.getTotalNodes(); i++)
    {
        Node *node = airspace.getNode(i);
        if (node != NULL && node->type == WAYPOINT)
        {
            radar.placeWaypoint(node->x, node->y, node->name, true);
        }
    }

    // Place active flights
    for (int i = 0; i < flightMgr.getTotalFlights(); i++)
    {
        ActiveFlight *f = flightMgr.getFlightByIndex(i);
        if (f != NULL)
        {
            Node *node = airspace.getNode(f->currentNodeID);
            if (node != NULL)
            {
                radar.placePlane(node->x, node->y, f->flightID, true);
            }
        }
    }
}

// ========== DISPLAY AIRSPACE VISUALIZER ==========
void displayAirspaceVisualizer()
{
    clearScreen();

    // Update radar with current flight positions
    updateRadarWithFlights();

    // Display radar
    radar.displayRadar();

    // Display nodes info
    cout << Color::BOLD << Color::MAGENTA
         << "\n+---------- AIRSPACE INFORMATION ----------+\n"
         << Color::RESET;

    for (int i = 1; i <= airspace.getTotalNodes(); i++)
    {
        Node *node = airspace.getNode(i);
        if (node != NULL)
        {
            string type = (node->type == AIRPORT) ? "AIRPORT" : "WAYPOINT";
            cout << "  " << Color::CYAN << "ID: " << node->id
                 << Color::RESET << " | Name: " << Color::GREEN << node->name
                 << Color::RESET << " | Type: " << Color::YELLOW << type
                 << Color::RESET << " | Pos: (" << node->x << ", " << node->y << ")";

            if (node->occupiedBy != "")
            {
                cout << " | " << Color::RED << "OCCUPIED by " << node->occupiedBy << Color::RESET;
            }
            cout << "\n";
        }
    }

    cout << Color::BOLD << Color::MAGENTA
         << "+------------------------------------------+\n"
         << Color::RESET << endl;

    cout << Color::YELLOW << "Press ENTER to return to menu..." << Color::RESET;
    cin.get();
}

// ========== FLIGHT CONTROL MENU ==========
void flightControlMenu()
{
    clearScreen();

    cout << Color::BOLD << Color::CYAN;
    cout << "\n+==============================================================+\n";
    cout << "|                   " << Color::YELLOW << "FLIGHT CONTROL PANEL" << Color::CYAN << "                      |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;

    cout << Color::BOLD << "  +-----------------------------------------------+\n";
    cout << "  | " << Color::RESET << Color::GREEN << "1. Add New Flight" << Color::RESET << Color::BOLD << "                             |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "2. Declare Emergency" << Color::RESET << Color::BOLD << "                          |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "3. Move Flight" << Color::RESET << Color::BOLD << "                                |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "4. Update Fuel Level" << Color::RESET << Color::BOLD << "                          |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "5. View Active Flights" << Color::RESET << Color::BOLD << "                        |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "6. Land Aircraft" << Color::RESET << Color::BOLD << "                              |\n";
    cout << "  | " << Color::RESET << Color::RED << "7. Return to Main Menu" << Color::RESET << Color::BOLD << "                        |\n";
    cout << "  +-----------------------------------------------+\n";
    cout << Color::RESET << endl;

    int choice;
    cout << Color::CYAN << "Enter your choice (1-7): " << Color::RESET;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        string flightID, startNodeName, destNodeName;
        int priority;

        cout << Color::CYAN << "\n[ADD FLIGHT]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);

        cout << "  " << Color::YELLOW << "Available Airports: JFK, LHR, CDG, DXB" << Color::RESET << "\n";
        cout << "  Enter Start Airport (e.g., JFK): ";
        getline(cin, startNodeName);

        cout << "  Enter Destination Airport (e.g., LHR): ";
        getline(cin, destNodeName);

        cout << "  " << Color::YELLOW << "Priority Levels:" << Color::RESET << " 1=Critical | 2=Low Fuel | 3=Normal | 4=Low\n";
        cout << "  Enter Priority (1-4): ";
        cin >> priority;

        int startNode = airspace.getNodeID(startNodeName);
        int destNode = airspace.getNodeID(destNodeName);

        if (startNode != -1 && destNode != -1)
        {
            flightMgr.addFlight(flightID, startNode, destNode, priority, "Aircraft");
        }
        else
        {
            cout << Color::RED << "[ERROR]" << Color::RESET << " Invalid airport names!" << endl;
        }
        break;
    }
    case 2:
    {
        string flightID;
        int newPriority;

        cout << Color::RED << "\n[DECLARE EMERGENCY]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);

        cout << "  " << Color::YELLOW << "New Priority:" << Color::RESET << " 1=Critical | 2=Low Fuel | 3=Normal | 4=Low\n";
        cout << "  Enter New Priority (1-4): ";
        cin >> newPriority;

        flightMgr.declareEmergency(flightID, newPriority);
        break;
    }
    case 3:
    {
        string flightID;

        cout << Color::CYAN << "\n[MOVE FLIGHT]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);

        flightMgr.moveFlightToNextNode(flightID);
        break;
    }
    case 4:
    {
        string flightID;
        int newFuel;

        cout << Color::YELLOW << "\n[UPDATE FUEL LEVEL]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);

        cout << "  Enter New Fuel Level (0-100%): ";
        cin >> newFuel;

        flightMgr.updateFuel(flightID, newFuel);
        break;
    }
    case 5:
    {
        flightMgr.displayAllActiveFlights();
        cout << Color::YELLOW << "Press ENTER to return..." << Color::RESET;
        cin.get();
        break;
    }
    case 6:
    {
        string flightID;

        cout << Color::GREEN << "\n[LAND AIRCRAFT]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);

        flightMgr.landFlight(flightID);
        break;
    }
    case 7:
        break;
    default:
        cout << Color::RED << "\n[ERROR] Invalid choice!\n"
             << Color::RESET;
    }
    cin.ignore();
    cout << Color::YELLOW << "\nPress ENTER to return..." << Color::RESET;
    cin.get();
}

// ========== SEARCH AND REPORTING MENU ==========
void searchAndReportingMenu()
{
    clearScreen();

    cout << Color::BOLD << Color::CYAN;
    cout << "\n+==============================================================+\n";
    cout << "|              " << Color::YELLOW << "SEARCH & REPORTING SYSTEM" << Color::CYAN << "                       |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;

    cout << Color::BOLD << "  +-----------------------------------------------+\n";
    cout << "  | " << Color::RESET << Color::GREEN << "1. Search Flight" << Color::RESET << Color::BOLD << "                              |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "2. Print Flight Log" << Color::RESET << Color::BOLD << "                           |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "3. Find Safe Route" << Color::RESET << Color::BOLD << "                            |\n";
    cout << "  | " << Color::RESET << Color::RED << "4. Return to Main Menu" << Color::RESET << Color::BOLD << "                        |\n";
    cout << "  +-----------------------------------------------+\n";
    cout << Color::RESET << endl;

    int choice;
    cout << Color::CYAN << "Enter your choice (1-4): " << Color::RESET;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        string flightID;
        cout << Color::GREEN << "\n[SEARCH FLIGHT]\n"
             << Color::RESET;
        cout << "  " << Color::YELLOW << "Example Flight ID: PK-786, BA-205, EK-156" << Color::RESET << "\n";
        cout << "  Enter Flight ID: ";
        getline(cin, flightID);
        flightMgr.displayFlightDetails(flightID);
        break;
    }
    case 2:
        flightLog.printAllLogs();
        break;
    case 3:
    {
        string nodeName;
        cout << Color::YELLOW << "\n[FIND SAFE ROUTE]\n"
             << Color::RESET;
        cout << "  " << Color::CYAN << "Available Locations: JFK, LHR, CDG, DXB, W1, W2, W3, W4" << Color::RESET << "\n";
        cout << "  Enter Current Location (e.g., JFK): ";
        getline(cin, nodeName);

        int nodeID = airspace.getNodeID(nodeName);
        if (nodeID != -1)
        {
            int nearest = airspace.findNearestAirport(nodeID);
            if (nearest != -1)
            {
                int path[MAX_NODES];
                int pathLength = 0;
                airspace.getPath(nodeID, nearest, path, pathLength);
                airspace.displayPath(path, pathLength);
            }
        }
        break;
    }
    case 4:
        return;
    default:
        cout << Color::RED << "\n[ERROR] Invalid choice!\n"
             << Color::RESET;
    }

    cout << Color::YELLOW << "\nPress ENTER to return..." << Color::RESET;
    cin.get();
}

// ========== SYSTEM MANAGEMENT MENU ==========
void systemMenu()
{
    clearScreen();

    cout << Color::BOLD << Color::CYAN;
    cout << "\n+==============================================================+\n";
    cout << "|               " << Color::YELLOW << "SYSTEM MANAGEMENT" << Color::CYAN << "                              |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;

    cout << Color::BOLD << "  +-----------------------------------------------+\n";
    cout << "  | " << Color::RESET << Color::GREEN << "1. Add Airport/Waypoint" << Color::RESET << Color::BOLD << "                       |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "2. Add Flight Corridor (Edge)" << Color::RESET << Color::BOLD << "                |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "3. View All Nodes & Edges" << Color::RESET << Color::BOLD << "                    |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "4. Save System Data" << Color::RESET << Color::BOLD << "                           |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "5. Load System Data" << Color::RESET << Color::BOLD << "                           |\n";
    cout << "  | " << Color::RESET << Color::GREEN << "6. System Status" << Color::RESET << Color::BOLD << "                              |\n";
    cout << "  | " << Color::RESET << Color::RED << "7. Return to Main Menu" << Color::RESET << Color::BOLD << "                        |\n";
    cout << "  +-----------------------------------------------+\n";
    cout << Color::RESET << endl;

    int choice;
    cout << Color::CYAN << "Enter your choice (1-7): " << Color::RESET;
    cin >> choice;
    cin.ignore(); // Clear newline from buffer

    switch (choice)
    {
    case 1:
    {
        int id, x, y, typeChoice;
        string name;
        NodeType type;

        cout << Color::CYAN << "\n[ADD NODE]\n"
             << Color::RESET;
        cout << "  Enter Node ID (unique number): ";
        cin >> id;
        cin.ignore();

        cout << "  Enter Node Name (e.g., LAX, W5): ";
        getline(cin, name);

        cout << "  Enter X coordinate (0-29): ";
        cin >> x;

        cout << "  Enter Y coordinate (0-19): ";
        cin >> y;

        cout << "  " << Color::YELLOW << "Node Type:" << Color::RESET << " 1=Airport | 2=Waypoint\n";
        cout << "  Enter Type (1-2): ";
        cin >> typeChoice;

        type = (typeChoice == 1) ? AIRPORT : WAYPOINT;

        airspace.addNode(id, name, x, y, type);
        break;
    }
    case 2:
    {
        int fromID, toID, weight;

        cout << Color::CYAN << "\n[ADD FLIGHT CORRIDOR]\n"
             << Color::RESET;
        cout << "  Enter Source Node ID: ";
        cin >> fromID;

        cout << "  Enter Destination Node ID: ";
        cin >> toID;

        cout << "  Enter Distance/Weight (e.g., 50, 100): ";
        cin >> weight;

        airspace.addEdge(fromID, toID, weight);
        break;
    }
    case 3:
    {
        cout << Color::CYAN << "\n[AIRSPACE NETWORK INFORMATION]\n"
             << Color::RESET;
        airspace.displayGraph();
        break;
    }
    case 4:
        flightMgr.saveToFile("atc_data.txt");
        break;
    case 5:
        flightMgr.loadFromFile("atc_data.txt");
        break;
    case 6:
        cout << Color::CYAN << "\n[INFO] All systems operational\n"
             << Color::RESET;
        cout << "  Active Flights: " << flightMgr.getTotalFlights() << endl;
        cout << "  Total Airports: 4" << endl;
        cout << "  Total Waypoints: 4" << endl;
        break;
    case 7:
        return;
    default:
        cout << Color::RED << "\n[ERROR] Invalid choice!\n"
             << Color::RESET;
    }

    cout << Color::YELLOW << "\nPress ENTER to return..." << Color::RESET;
    cin.get();
}

// ========== DISPLAY FOOTER ==========
void displayFooter()
{
    cout << Color::BOLD << Color::BLUE;
    cout << "\n+==============================================================+\n";
    cout << "|  SkyNet ATC Simulator v1.0 | Data Structures Project      |\n";
    cout << "|  Thank you for using SkyNet ATC System                    |\n";
    cout << "+==============================================================+\n";
    cout << Color::RESET << endl;
}

// ========== MAIN PROGRAM ==========
int main()
{
    int choice;

    // Display welcome screen
    displayWelcome();

    // Initialize airspace
    initializeAirspace();

    cout << Color::YELLOW << "Press ENTER to start..." << Color::RESET;
    cin.ignore();

    // Main program loop
    while (true)
    {
        displayMainMenu();
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        switch (choice)
        {
        case 1:
            displayAirspaceVisualizer();
            break;
        case 2:
            flightControlMenu();
            break;
        case 3:
            searchAndReportingMenu();
            break;
        case 4:
            systemMenu();
            break;
        case 5:
            clearScreen();
            cout << Color::BOLD << Color::CYAN;
            cout << "\n+==============================================================+\n";
            cout << "|               " << Color::YELLOW << "AIRCRAFT REGISTRY" << Color::CYAN << "                              |\n";
            cout << "+==============================================================+\n";
            cout << Color::RESET << endl;
            registry.displayAllAircraft();
            cout << Color::YELLOW << "Press ENTER to return to menu..." << Color::RESET;
            cin.get();
            break;
        case 6:
            clearScreen();
            displayFooter();
            cout << Color::GREEN << "Program terminated successfully.\n"
                 << Color::RESET;
            return 0;
        default:
            cout << Color::RED << "\n[ERROR] Invalid choice! Please try again.\n"
                 << Color::RESET;
            cout << Color::YELLOW << "Press ENTER to continue..." << Color::RESET;
            cin.ignore();
        }
    }

    return 0;
}
#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <string>
#include <iostream>
#include "Graph.h"

using namespace std;

const int MAX_ACTIVE_FLIGHTS = 20;

// ========== FLIGHT STATUS ENUM ==========
enum FlightStatus
{
    TAXIING,
    FLYING,
    APPROACHING,
    LANDING,
    LANDED,
    CRASHED,
    GROUNDED
};

// ========== ACTIVE FLIGHT STRUCTURE ==========
struct ActiveFlight
{
    string flightID;
    int currentNodeID;
    int destinationNodeID;
    int priority; // 1=Critical, 2=Low Fuel, 3=Normal, 4=Low
    int fuel;     // 0-100%
    FlightStatus status;
    int pathIndex;       // Current position in path
    int path[MAX_NODES]; // Flight path
    int pathLength;

    ActiveFlight() : flightID(""), currentNodeID(-1), destinationNodeID(-1),
                     priority(3), fuel(100), status(GROUNDED),
                     pathIndex(0), pathLength(0) {}
};

// ========== FLIGHT MANAGER CLASS ==========
class FlightManager
{
private:
    ActiveFlight flights[MAX_ACTIVE_FLIGHTS];
    int totalFlights;
    Graph *graphPtr; // Pointer to graph for coordinate access

public:
    // Constructor & Destructor
    FlightManager();
    ~FlightManager();

    // Set graph reference
    void setGraphReference(Graph *g);

    // Flight creation
    bool addFlight(string flightID, int startNodeID, int destNodeID,
                   int priority, string model);

    // Flight removal
    bool removeFlight(string flightID);

    // Find flight
    ActiveFlight *getFlight(string flightID);

    // Flight movement (with collision detection)
    bool moveFlightToNextNode(string flightID);

    // Emergency/Priority updates
    bool declareEmergency(string flightID, int newPriority);
    bool updateFuel(string flightID, int newFuel);

    // Flight status
    FlightStatus getFlightStatus(string flightID);
    bool landFlight(string flightID);

    // Display functions
    void displayAllActiveFlights();
    void displayFlightDetails(string flightID);
    void displayFlightPath(string flightID);

    // Query functions
    int getTotalFlights();
    bool flightExists(string flightID);
    int getFlightNodeID(string flightID);

    // Accessor for iterating active flights (read-only pointer)
    ActiveFlight *getFlightByIndex(int index);

    // Collision checking
    bool checkCollision(int nodeID, string excludeFlightID = "");

    // Save/Load functions
    void saveToFile(string filename);
    void loadFromFile(string filename);
};

#endif // FLIGHT_MANAGER_H
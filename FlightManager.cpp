#include "FlightManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>

using namespace std;

// ========== COLOR CODES ==========
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";
const string BOLD = "\033[1m";

// ========== CONSTRUCTOR ==========
FlightManager::FlightManager()
{
    totalFlights = 0;
    graphPtr = NULL;
    registryPtr = NULL;
    cout << GREEN << "[SUCCESS] Flight Manager initialized" << RESET << endl;
}

// ========== DESTRUCTOR ==========
FlightManager::~FlightManager()
{
    // Nothing special to clean up
}

// ========== SET GRAPH REFERENCE ==========
void FlightManager::setGraphReference(Graph *g)
{
    graphPtr = g;
    cout << GREEN << "[SUCCESS] Graph reference set" << RESET << endl;
}

// ========== SET REGISTRY REFERENCE ==========
void FlightManager::setRegistryReference(HashTable *reg)
{
    registryPtr = reg;
    cout << GREEN << "[SUCCESS] Registry reference set" << RESET << endl;
}

// ========== ADD FLIGHT ==========
bool FlightManager::addFlight(string flightID, int startNodeID, int destNodeID,
                              int priority, string model)
{
    if (totalFlights >= MAX_ACTIVE_FLIGHTS)
    {
        cout << RED << "[ERROR]" << RESET << " Maximum active flights reached!" << endl;
        return false;
    }

    if (flightExists(flightID))
    {
        cout << RED << "[ERROR]" << RESET << " Flight " << CYAN << flightID
             << RESET << " already exists!" << endl;
        return false;
    }

    if (graphPtr == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Graph reference not set!" << endl;
        return false;
    }

    // Validate nodes
    Node *startNode = graphPtr->getNode(startNodeID);
    Node *destNode = graphPtr->getNode(destNodeID);

    if (startNode == NULL || destNode == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Invalid node ID!" << endl;
        return false;
    }

    // Create flight
    ActiveFlight newFlight;
    newFlight.flightID = flightID;
    newFlight.currentNodeID = startNodeID;
    newFlight.destinationNodeID = destNodeID;
    newFlight.priority = priority;
    newFlight.fuel = 100;
    newFlight.status = FLYING;

    // Calculate path using Dijkstra
    graphPtr->getPath(startNodeID, destNodeID, newFlight.path, newFlight.pathLength);

    if (newFlight.pathLength == 0)
    {
        cout << RED << "[ERROR]" << RESET << " No path exists to destination!" << endl;
        return false;
    }

    // Occupy starting node
    graphPtr->occupyNode(startNodeID, flightID);

    // Add flight
    flights[totalFlights] = newFlight;
    totalFlights++;

    // Add aircraft to registry if registry is available
    if (registryPtr != NULL)
    {
        registryPtr->insertAircraft(flightID, model, 100, startNode->name, destNode->name, "FLYING");
    }

    cout << GREEN << "[ADD]" << RESET << " Flight " << CYAN << flightID
         << RESET << " added from " << startNode->name << " to " << destNode->name << endl;

    return true;
}

// ========== REMOVE FLIGHT ==========
bool FlightManager::removeFlight(string flightID)
{
    for (int i = 0; i < totalFlights; i++)
    {
        if (flights[i].flightID == flightID)
        {
            // Free the node
            graphPtr->freeNode(flights[i].currentNodeID);

            // Shift array
            for (int j = i; j < totalFlights - 1; j++)
            {
                flights[j] = flights[j + 1];
            }

            totalFlights--;
            cout << GREEN << "[REMOVE]" << RESET << " Flight " << CYAN << flightID
                 << RESET << " removed" << endl;
            return true;
        }
    }

    cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
    return false;
}

// ========== GET FLIGHT ==========
ActiveFlight *FlightManager::getFlight(string flightID)
{
    for (int i = 0; i < totalFlights; i++)
    {
        if (flights[i].flightID == flightID)
        {
            return &flights[i];
        }
    }
    return NULL;
}

// ========== MOVE FLIGHT TO NEXT NODE (COLLISION DETECTION) ==========
bool FlightManager::moveFlightToNextNode(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return false;
    }

    if (flight->pathIndex >= flight->pathLength - 1)
    {
        cout << YELLOW << "[INFO]" << RESET << " Flight " << CYAN << flightID
             << RESET << " has reached destination!" << endl;
        return false;
    }

    // Get next node in path
    int nextNodeID = flight->path[flight->pathIndex + 1];

    // CHECK COLLISION - This is Module E!
    if (checkCollision(nextNodeID, flightID))
    {
        cout << RED << "[ALERT]" << RESET << " Flight " << CYAN << flightID
             << RESET << " cannot move - sector occupied!" << endl;
        cout << RED << "[COLLISION]" << RESET << " Holding flight at current position" << endl;
        return false;
    }

    // Free old node
    graphPtr->freeNode(flight->currentNodeID);

    // Move to next node
    flight->currentNodeID = nextNodeID;
    flight->pathIndex++;

    // Occupy new node
    graphPtr->occupyNode(nextNodeID, flightID);

    // Decrease fuel
    flight->fuel = max(0, flight->fuel - 1);

    cout << CYAN << "[MOVE]" << RESET << " Flight " << CYAN << flightID
         << RESET << " moved to node " << nextNodeID
         << " (Fuel: " << flight->fuel << "%)" << endl;

    // Check if reached destination
    if (flight->pathIndex == flight->pathLength - 1)
    {
        flight->status = APPROACHING;
        cout << YELLOW << "[APPROACHING]" << RESET << " Flight " << CYAN << flightID
             << RESET << " approaching destination!" << endl;
    }

    return true;
}

// ========== DECLARE EMERGENCY ==========
bool FlightManager::declareEmergency(string flightID, int newPriority)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return false;
    }

    int oldPriority = flight->priority;
    flight->priority = newPriority;

    string priorityName;
    if (newPriority == 1)
        priorityName = "CRITICAL";
    else if (newPriority == 2)
        priorityName = "LOW FUEL";
    else if (newPriority == 3)
        priorityName = "NORMAL";
    else
        priorityName = "LOW";

    cout << RED << "[EMERGENCY]" << RESET << " Flight " << CYAN << flightID
         << RESET << " priority changed from " << oldPriority << " to "
         << newPriority << " (" << priorityName << ")" << endl;

    return true;
}

// ========== UPDATE FUEL ==========
bool FlightManager::updateFuel(string flightID, int newFuel)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return false;
    }

    if (newFuel < 0 || newFuel > 100)
    {
        cout << RED << "[ERROR]" << RESET << " Invalid fuel value!" << endl;
        return false;
    }

    flight->fuel = newFuel;

    if (newFuel < 10)
    {
        cout << RED << "[ALERT]" << RESET << " Flight " << CYAN << flightID
             << RESET << " LOW FUEL: " << newFuel << "%" << endl;
    }
    else
    {
        cout << YELLOW << "[UPDATE]" << RESET << " Flight fuel updated to "
             << newFuel << "%" << endl;
    }

    return true;
}

// ========== LAND FLIGHT ==========
bool FlightManager::landFlight(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return false;
    }

    if (flight->status != APPROACHING)
    {
        cout << RED << "[ERROR]" << RESET << " Flight must be approaching to land!" << endl;
        return false;
    }

    flight->status = LANDED;

    // Update registry status if available
    if (registryPtr != NULL)
    {
        registryPtr->updateStatus(flightID, "LANDED");
    }

    cout << GREEN << "[LANDED]" << RESET << " Flight " << CYAN << flightID
         << RESET << " has successfully landed!" << endl;

    removeFlight(flightID);
    return true;
}

// ========== GET FLIGHT STATUS ==========
FlightStatus FlightManager::getFlightStatus(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);
    if (flight != NULL)
    {
        return flight->status;
    }
    return GROUNDED;
}

// ========== DISPLAY ALL ACTIVE FLIGHTS ==========
void FlightManager::displayAllActiveFlights()
{
    cout << BOLD << CYAN;
    cout << "\n+=============================================================+\n";
    cout << "|              " << YELLOW << "ACTIVE FLIGHTS" << CYAN << "                            |\n";
    cout << "+=============================================================+\n";
    cout << RESET << endl;

    if (totalFlights == 0)
    {
        cout << YELLOW << "  No active flights\n"
             << RESET << endl;
        return;
    }

    cout << BOLD << "  +----------------------------------------------------------+\n";
    cout << "  | " << RESET << "Total Active: " << CYAN << totalFlights << RESET << BOLD << "                               |\n";
    cout << "  +----------------------------------------------------------+\n"
         << RESET << endl;

    for (int i = 0; i < totalFlights; i++)
    {
        cout << BOLD << "  " << (i + 1) << ". " << RESET;
        cout << CYAN << setw(12) << flights[i].flightID << RESET << " | ";
        cout << GREEN << setw(10) << flights[i].currentNodeID << RESET << " | ";
        cout << YELLOW << "Fuel: " << setw(3) << flights[i].fuel << "% " << RESET << "| ";

        string status;
        if (flights[i].status == FLYING)
            status = "FLYING";
        else if (flights[i].status == APPROACHING)
            status = "APPROACHING";
        else if (flights[i].status == LANDED)
            status = "LANDED";
        else if (flights[i].status == CRASHED)
            status = "CRASHED";

        cout << status << endl;
    }

    cout << endl;
}

// ========== DISPLAY FLIGHT DETAILS ==========
void FlightManager::displayFlightDetails(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return;
    }

    cout << BOLD << CYAN << "\n+------------- FLIGHT DETAILS ----------+\n"
         << RESET;

    cout << "  " << BOLD << "Flight ID: " << RESET << CYAN << flight->flightID << RESET << endl;
    cout << "  " << BOLD << "Current Node: " << RESET << flight->currentNodeID << endl;
    cout << "  " << BOLD << "Destination Node: " << RESET << flight->destinationNodeID << endl;
    cout << "  " << BOLD << "Priority: " << RESET << flight->priority << endl;
    cout << "  " << BOLD << "Fuel: " << RESET << YELLOW << flight->fuel << "%" << RESET << endl;
    cout << "  " << BOLD << "Path Progress: " << RESET << flight->pathIndex + 1 << "/" << flight->pathLength << endl;

    cout << BOLD << CYAN << "+----------------------------------------+\n"
         << RESET << endl;
}

// ========== DISPLAY FLIGHT PATH ==========
void FlightManager::displayFlightPath(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);

    if (flight == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight not found!" << endl;
        return;
    }

    cout << "\n"
         << BOLD << CYAN << "+---------- FLIGHT PATH ----------+\n"
         << RESET;
    cout << "  Path: ";

    for (int i = 0; i < flight->pathLength; i++)
    {
        cout << flight->path[i];
        if (i < flight->pathLength - 1)
            cout << " -> ";
    }

    cout << "\n"
         << BOLD << CYAN << "+----------------------------------+\n"
         << RESET << endl;
}

// ========== GET TOTAL FLIGHTS ==========
int FlightManager::getTotalFlights()
{
    return totalFlights;
}

// ========== GET FLIGHT BY INDEX (READ-ONLY) ==========
ActiveFlight *FlightManager::getFlightByIndex(int index)
{
    if (index < 0 || index >= totalFlights)
    {
        return NULL;
    }
    return &flights[index];
}

// ========== CHECK IF FLIGHT EXISTS ==========
bool FlightManager::flightExists(string flightID)
{
    return getFlight(flightID) != NULL;
}

// ========== GET FLIGHT NODE ID ==========
int FlightManager::getFlightNodeID(string flightID)
{
    ActiveFlight *flight = getFlight(flightID);
    if (flight != NULL)
    {
        return flight->currentNodeID;
    }
    return -1;
}

// ========== CHECK COLLISION (MODULE E IMPLEMENTATION) ==========
bool FlightManager::checkCollision(int nodeID, string excludeFlightID)
{
    for (int i = 0; i < totalFlights; i++)
    {
        if (flights[i].flightID != excludeFlightID &&
            flights[i].currentNodeID == nodeID)
        {
            return true; // Collision detected!
        }
    }
    return false; // No collision
}

// ========== SAVE TO FILE ==========
void FlightManager::saveToFile(string filename)
{
    ofstream file(filename.c_str());

    if (!file.is_open())
    {
        cout << RED << "[ERROR]" << RESET << " Could not open file for saving!" << endl;
        return;
    }

    file << totalFlights << endl;

    for (int i = 0; i < totalFlights; i++)
    {
        file << flights[i].flightID << endl;
        file << flights[i].currentNodeID << endl;
        file << flights[i].destinationNodeID << endl;
        file << flights[i].priority << endl;
        file << flights[i].fuel << endl;
        file << flights[i].status << endl;
        file << flights[i].pathIndex << endl;
        file << flights[i].pathLength << endl;

        for (int j = 0; j < flights[i].pathLength; j++)
        {
            file << flights[i].path[j] << " ";
        }
        file << endl;
    }

    file.close();
    cout << GREEN << "[SUCCESS]" << RESET << " Flight data saved to " << filename << endl;
}

// ========== LOAD FROM FILE ==========
void FlightManager::loadFromFile(string filename)
{
    ifstream file(filename.c_str());

    if (!file.is_open())
    {
        cout << RED << "[ERROR]" << RESET << " Could not open file for loading!" << endl;
        return;
    }

    file >> totalFlights;
    file.ignore();

    for (int i = 0; i < totalFlights; i++)
    {
        getline(file, flights[i].flightID);
        file >> flights[i].currentNodeID;
        file >> flights[i].destinationNodeID;
        file >> flights[i].priority;
        file >> flights[i].fuel;

        int statusValue;
        file >> statusValue;
        flights[i].status = static_cast<FlightStatus>(statusValue);

        file >> flights[i].pathIndex;
        file >> flights[i].pathLength;

        for (int j = 0; j < flights[i].pathLength; j++)
        {
            file >> flights[i].path[j];
        }
        file.ignore();
    }

    file.close();
    cout << GREEN << "[SUCCESS]" << RESET << " Flight data loaded from " << filename << endl;
    cout << CYAN << "[INFO]" << RESET << " Loaded " << totalFlights << " active flights" << endl;
}
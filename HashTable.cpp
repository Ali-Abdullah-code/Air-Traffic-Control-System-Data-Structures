#include "HashTable.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ========== COLOR CODES ==========
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";
const string BOLD = "\033[1m";

// ========== HASH FUNCTION ==========
int HashTable::hashFunction(string flightID)
{
    int sum = 0;

    // Sum all ASCII values of characters in flightID
    for (int i = 0; i < flightID.length(); i++)
    {
        sum += (int)flightID[i];
    }

    return sum % HASH_TABLE_SIZE;
}

// ========== CONSTRUCTOR ==========
HashTable::HashTable()
{
    totalAircraft = 0;

    // Initialize all buckets to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        buckets[i] = NULL;
    }

    cout << GREEN << "[SUCCESS] Hash Table initialized" << RESET << endl;
}

// ========== DESTRUCTOR ==========
HashTable::~HashTable()
{
    // Delete all nodes in all buckets
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode *current = buckets[i];
        while (current != NULL)
        {
            HashNode *temp = current;
            current = current->next;
            delete temp;
        }
    }

    cout << GREEN << "[SUCCESS] Hash Table destroyed" << RESET << endl;
}

// ========== CLEAR SCREEN HELPER ==========
void HashTable::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ========== INSERT/UPDATE AIRCRAFT ==========
void HashTable::insertAircraft(string flightID, string model, int fuel,
                               string origin, string destination, string status)
{

    // Calculate hash
    int bucket = hashFunction(flightID);

    // Check if aircraft already exists
    HashNode *current = buckets[bucket];
    while (current != NULL)
    {
        if (current->aircraft.flightID == flightID)
        {
            // Aircraft exists, update it
            current->aircraft.model = model;
            current->aircraft.fuel = fuel;
            current->aircraft.origin = origin;
            current->aircraft.destination = destination;
            current->aircraft.status = status;

            cout << YELLOW << "[UPDATE]" << RESET << " Flight " << CYAN << flightID
                 << RESET << " updated successfully" << endl;
            return;
        }
        current = current->next;
    }

    // Aircraft doesn't exist, create new node
    Aircraft newAircraft(flightID, model, fuel, origin, destination, status);
    HashNode *newNode = new HashNode(newAircraft);

    // Insert at the beginning of the chain
    newNode->next = buckets[bucket];
    buckets[bucket] = newNode;
    totalAircraft++;

    cout << GREEN << "[INSERT]" << RESET << " Flight " << CYAN << flightID
         << RESET << " added to registry" << endl;
}

// ========== SEARCH AIRCRAFT ==========
Aircraft *HashTable::searchAircraft(string flightID)
{
    // Calculate hash
    int bucket = hashFunction(flightID);

    // Traverse the chain at this bucket
    HashNode *current = buckets[bucket];
    while (current != NULL)
    {
        if (current->aircraft.flightID == flightID)
        {
            cout << GREEN << "[FOUND]" << RESET << " Flight " << CYAN << flightID
                 << RESET << " located in database" << endl;
            return &(current->aircraft);
        }
        current = current->next;
    }

    // Aircraft not found
    cout << RED << "[NOT FOUND]" << RESET << " Flight " << CYAN << flightID
         << RESET << " does not exist" << endl;
    return NULL;
}

// ========== UPDATE STATUS ==========
void HashTable::updateStatus(string flightID, string newStatus)
{
    Aircraft *aircraft = searchAircraft(flightID);

    if (aircraft != NULL)
    {
        aircraft->status = newStatus;
        cout << YELLOW << "[UPDATE]" << RESET << " Status changed to "
             << CYAN << newStatus << RESET << endl;
    }
    else
    {
        cout << RED << "[ERROR]" << RESET << " Cannot update status - flight not found" << endl;
    }
}

// ========== UPDATE FUEL ==========
void HashTable::updateFuel(string flightID, int newFuel)
{
    Aircraft *aircraft = searchAircraft(flightID);

    if (aircraft != NULL)
    {
        if (newFuel < 0 || newFuel > 100)
        {
            cout << RED << "[ERROR]" << RESET << " Fuel must be between 0-100%" << endl;
            return;
        }

        aircraft->fuel = newFuel;
        cout << YELLOW << "[UPDATE]" << RESET << " Fuel updated to "
             << CYAN << newFuel << "%" << RESET << endl;

        // Alert if fuel is low
        if (newFuel < 10)
        {
            cout << RED << "[ALERT]" << RESET << " Low fuel warning! "
                 << "Fuel at " << newFuel << "%" << endl;
        }
    }
    else
    {
        cout << RED << "[ERROR]" << RESET << " Cannot update fuel - flight not found" << endl;
    }
}

// ========== DELETE AIRCRAFT ==========
bool HashTable::deleteAircraft(string flightID)
{
    int bucket = hashFunction(flightID);

    // Special case: if head of chain matches
    if (buckets[bucket] != NULL && buckets[bucket]->aircraft.flightID == flightID)
    {
        HashNode *temp = buckets[bucket];
        buckets[bucket] = buckets[bucket]->next;
        delete temp;
        totalAircraft--;

        cout << GREEN << "[DELETE]" << RESET << " Flight " << CYAN << flightID
             << RESET << " removed from registry" << endl;
        return true;
    }

    // Search in the chain
    HashNode *current = buckets[bucket];
    while (current != NULL && current->next != NULL)
    {
        if (current->next->aircraft.flightID == flightID)
        {
            HashNode *temp = current->next;
            current->next = current->next->next;
            delete temp;
            totalAircraft--;

            cout << GREEN << "[DELETE]" << RESET << " Flight " << CYAN << flightID
                 << RESET << " removed from registry" << endl;
            return true;
        }
        current = current->next;
    }

    cout << RED << "[ERROR]" << RESET << " Flight " << CYAN << flightID
         << RESET << " not found in registry" << endl;
    return false;
}

// ========== DISPLAY ALL AIRCRAFT ==========
void HashTable::displayAllAircraft()
{
    clearScreen();

    cout << BOLD << CYAN;
    cout << "\n+=============================================================+\n";
    cout << "|              " << YELLOW << "AIRCRAFT REGISTRY" << CYAN << "                        |\n";
    cout << "+=============================================================+\n";
    cout << RESET << endl;

    if (totalAircraft == 0)
    {
        cout << YELLOW << "  No aircraft in registry\n"
             << RESET << endl;
        return;
    }

    cout << BOLD << "  +----------------------------------------------------------+\n";
    cout << "  | " << RESET << "Total Aircraft: " << CYAN << totalAircraft << RESET << BOLD << "                             |\n";
    cout << "  +----------------------------------------------------------+\n"
         << RESET << endl;

    int count = 1;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashNode *current = buckets[i];
        while (current != NULL)
        {
            cout << BOLD << "  " << count << ". " << RESET;
            cout << CYAN << setw(12) << current->aircraft.flightID << RESET << " | ";
            cout << GREEN << setw(15) << current->aircraft.model << RESET << " | ";
            cout << YELLOW << "Fuel: " << setw(3) << current->aircraft.fuel << "% " << RESET << "| ";
            cout << current->aircraft.origin << " -> " << current->aircraft.destination;
            cout << endl;

            current = current->next;
            count++;
        }
    }

    cout << endl;
}

// ========== DISPLAY SPECIFIC AIRCRAFT ==========
void HashTable::displayAircraft(string flightID)
{
    Aircraft *aircraft = searchAircraft(flightID);

    if (aircraft == NULL)
    {
        return;
    }

    cout << BOLD << CYAN << "\n+------------- AIRCRAFT DETAILS ----------+\n"
         << RESET;

    cout << "  " << BOLD << "Flight ID: " << RESET << CYAN << aircraft->flightID << RESET << endl;
    cout << "  " << BOLD << "Model: " << RESET << GREEN << aircraft->model << RESET << endl;
    cout << "  " << BOLD << "Fuel: " << RESET << YELLOW << aircraft->fuel << "%" << RESET << endl;
    cout << "  " << BOLD << "Origin: " << RESET << aircraft->origin << endl;
    cout << "  " << BOLD << "Destination: " << RESET << aircraft->destination << endl;
    cout << "  " << BOLD << "Status: " << RESET << aircraft->status << endl;

    cout << BOLD << CYAN << "+----------------------------------------+\n"
         << RESET << endl;
}

// ========== GET TOTAL AIRCRAFT COUNT ==========
int HashTable::getTotalAircraft()
{
    return totalAircraft;
}

// ========== CHECK IF AIRCRAFT EXISTS ==========
bool HashTable::exists(string flightID)
{
    int bucket = hashFunction(flightID);

    HashNode *current = buckets[bucket];
    while (current != NULL)
    {
        if (current->aircraft.flightID == flightID)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}
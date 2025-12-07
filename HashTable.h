#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>

using namespace std;

const int HASH_TABLE_SIZE = 100;

// ========== AIRCRAFT STRUCTURE ==========
struct Aircraft
{
    string flightID;
    string model;
    int fuel;
    string origin;
    string destination;
    string status;

    Aircraft() : flightID(""), model(""), fuel(0),
                 origin(""), destination(""), status("") {}

    Aircraft(string id, string m, int f, string org,
             string dest, string st)
        : flightID(id), model(m), fuel(f),
          origin(org), destination(dest), status(st) {}
};

// ========== HASH TABLE NODE (for chaining) ==========
struct HashNode
{
    Aircraft aircraft;
    HashNode *next;

    HashNode() : next(NULL) {}

    HashNode(Aircraft a) : aircraft(a), next(NULL) {}
};

// ========== HASH TABLE CLASS ==========
class HashTable
{
private:
    HashNode *buckets[HASH_TABLE_SIZE];
    int totalAircraft;

    // Hash function
    int hashFunction(string flightID);

public:
    // Constructor & Destructor
    HashTable();
    ~HashTable();

    // Insert/Update aircraft
    void insertAircraft(string flightID, string model, int fuel,
                        string origin, string destination, string status);

    // Search aircraft
    Aircraft *searchAircraft(string flightID);

    // Update aircraft status
    void updateStatus(string flightID, string newStatus);

    // Update fuel level
    void updateFuel(string flightID, int newFuel);

    // Delete aircraft
    bool deleteAircraft(string flightID);

    // Display all aircraft
    void displayAllAircraft();

    // Display specific aircraft
    void displayAircraft(string flightID);

    // Get total aircraft count
    int getTotalAircraft();

    // Check if aircraft exists
    bool exists(string flightID);

    void clearScreen();
};

#endif // HASHTABLE_H
#ifndef MINHEAP_H
#define MINHEAP_H

#include <string>
#include <iostream>

using namespace std;

const int MAX_FLIGHTS = 100;

// ========== FLIGHT STRUCTURE FOR HEAP ==========
struct Flight
{
    string flightID;
    int priority; // 1=Critical, 2=Low Fuel, 3=Normal, 4=Low

    Flight() : flightID(""), priority(4) {}

    Flight(string id, int p) : flightID(id), priority(p) {}

    void display()
    {
        cout << "Flight: " << flightID << " | Priority: " << priority;
    }
};

// ========== MIN HEAP CLASS ==========
class MinHeap
{
private:
    Flight flights[MAX_FLIGHTS]; // 1-indexed array
    int size;

    // Helper functions
    void heapifyUp(int index);
    void heapifyDown(int index);
    int getParent(int index);
    int getLeft(int index);
    int getRight(int index);
    bool hasHigherPriority(Flight a, Flight b);

public:
    // Constructor & Destructor
    MinHeap();
    ~MinHeap();

    // Insert flight into queue
    void insert(string flightID, int priority);

    // Extract minimum priority flight (land it)
    Flight extractMin();

    // Change priority of flight (emergency override)
    void updatePriority(string flightID, int newPriority);

    // Display landing queue
    void displayQueue();

    // Get next flight to land
    Flight getNextFlight();

    // Check if heap is empty
    bool isEmpty();

    // Get heap size
    int getSize();
};

#endif // MINHEAP_H
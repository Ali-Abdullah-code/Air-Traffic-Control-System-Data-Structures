#include "MinHeap.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ========== COLOR CODES ==========
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";
const string BOLD = "\033[1m";

// ========== CONSTRUCTOR ==========
MinHeap::MinHeap()
{
    size = 0;
    // Array is 1-indexed, so we start from index 1
    cout << GREEN << "[SUCCESS] Landing Queue initialized" << RESET << endl;
}

// ========== DESTRUCTOR ==========
MinHeap::~MinHeap()
{
    // Nothing to delete - array is static
}

// ========== GET PARENT INDEX ==========
int MinHeap::getParent(int index)
{
    return index / 2;
}

// ========== GET LEFT CHILD INDEX ==========
int MinHeap::getLeft(int index)
{
    return 2 * index;
}

// ========== GET RIGHT CHILD INDEX ==========
int MinHeap::getRight(int index)
{
    return 2 * index + 1;
}

// ========== CHECK WHICH HAS HIGHER PRIORITY ==========
bool MinHeap::hasHigherPriority(Flight a, Flight b)
{
    // Lower priority number = higher priority
    return a.priority < b.priority;
}

// ========== HEAPIFY UP ==========
void MinHeap::heapifyUp(int index)
{
    // Move element up until heap property is satisfied
    while (index > 1)
    {
        int parentIndex = getParent(index);

        // If current has higher priority than parent, swap
        if (hasHigherPriority(flights[index], flights[parentIndex]))
        {
            // Swap
            Flight temp = flights[index];
            flights[index] = flights[parentIndex];
            flights[parentIndex] = temp;

            index = parentIndex;
        }
        else
        {
            break;
        }
    }
}

// ========== HEAPIFY DOWN ==========
void MinHeap::heapifyDown(int index)
{
    // Move element down until heap property is satisfied
    while (true)
    {
        int leftIndex = getLeft(index);
        int rightIndex = getRight(index);
        int smallest = index;

        // Check if left child has higher priority
        if (leftIndex <= size && hasHigherPriority(flights[leftIndex], flights[smallest]))
        {
            smallest = leftIndex;
        }

        // Check if right child has higher priority
        if (rightIndex <= size && hasHigherPriority(flights[rightIndex], flights[smallest]))
        {
            smallest = rightIndex;
        }

        // If smallest is not current node, swap and continue
        if (smallest != index)
        {
            Flight temp = flights[index];
            flights[index] = flights[smallest];
            flights[smallest] = temp;

            index = smallest;
        }
        else
        {
            break;
        }
    }
}

// ========== INSERT FLIGHT INTO HEAP ==========
void MinHeap::insert(string flightID, int priority)
{
    if (size >= MAX_FLIGHTS)
    {
        cout << RED << "[ERROR]" << RESET << " Landing queue is full!" << endl;
        return;
    }

    // Validate priority
    if (priority < 1 || priority > 4)
    {
        cout << RED << "[ERROR]" << RESET << " Invalid priority! Use 1-4" << endl;
        return;
    }

    // Add flight at the end
    size++;
    flights[size] = Flight(flightID, priority);

    // Restore heap property
    heapifyUp(size);

    string priorityName;
    if (priority == 1)
        priorityName = "CRITICAL";
    else if (priority == 2)
        priorityName = "LOW FUEL";
    else if (priority == 3)
        priorityName = "NORMAL";
    else
        priorityName = "LOW";

    cout << GREEN << "[INSERT]" << RESET << " Flight " << CYAN << flightID
         << RESET << " added to queue [" << YELLOW << priorityName << RESET << "]" << endl;
}

// ========== EXTRACT MINIMUM (LAND FLIGHT) ==========
Flight MinHeap::extractMin()
{
    if (size == 0)
    {
        cout << RED << "[ERROR]" << RESET << " Landing queue is empty!" << endl;
        return Flight("", -1);
    }

    // Get the flight with highest priority
    Flight minFlight = flights[1];

    // Move last flight to root
    flights[1] = flights[size];
    size--;

    // Restore heap property
    if (size > 0)
    {
        heapifyDown(1);
    }

    return minFlight;
}

// ========== UPDATE PRIORITY (EMERGENCY OVERRIDE) ==========
void MinHeap::updatePriority(string flightID, int newPriority)
{
    if (newPriority < 1 || newPriority > 4)
    {
        cout << RED << "[ERROR]" << RESET << " Invalid priority! Use 1-4" << endl;
        return;
    }

    // Find the flight
    int index = -1;
    for (int i = 1; i <= size; i++)
    {
        if (flights[i].flightID == flightID)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << RED << "[ERROR]" << RESET << " Flight " << CYAN << flightID
             << RESET << " not found in queue" << endl;
        return;
    }

    // Update priority
    int oldPriority = flights[index].priority;
    flights[index].priority = newPriority;

    // Restore heap property
    if (newPriority < oldPriority)
    {
        // Priority increased (number decreased), move up
        heapifyUp(index);
    }
    else
    {
        // Priority decreased (number increased), move down
        heapifyDown(index);
    }

    string priorityName;
    if (newPriority == 1)
        priorityName = "CRITICAL";
    else if (newPriority == 2)
        priorityName = "LOW FUEL";
    else if (newPriority == 3)
        priorityName = "NORMAL";
    else
        priorityName = "LOW";

    cout << YELLOW << "[EMERGENCY]" << RESET << " Flight " << CYAN << flightID
         << RESET << " priority updated to " << RED << priorityName << RESET << endl;
}

// ========== GET NEXT FLIGHT TO LAND ==========
Flight MinHeap::getNextFlight()
{
    if (size == 0)
    {
        cout << RED << "[ERROR]" << RESET << " Landing queue is empty!" << endl;
        return Flight("", -1);
    }

    return flights[1];
}

// ========== CHECK IF HEAP IS EMPTY ==========
bool MinHeap::isEmpty()
{
    return size == 0;
}

// ========== GET HEAP SIZE ==========
int MinHeap::getSize()
{
    return size;
}

// ========== DISPLAY LANDING QUEUE ==========
void MinHeap::displayQueue()
{
    cout << BOLD << CYAN << "\n+=============================================================+\n";
    cout << "|              " << YELLOW << "LANDING PRIORITY QUEUE" << CYAN << "                     |\n";
    cout << "+=============================================================+\n"
         << RESET << endl;

    if (size == 0)
    {
        cout << YELLOW << "  No flights waiting to land\n"
             << RESET << endl;
        return;
    }

    cout << BOLD << "  +----------------------------------------------------------+\n";
    cout << "  | " << RESET << "Total Waiting: " << CYAN << size << RESET << BOLD
         << "                               |\n";
    cout << "  +----------------------------------------------------------+\n"
         << RESET << endl;

    // Create a copy of heap for display without modifying it
    MinHeap tempHeap = *this;
    int position = 1;

    while (tempHeap.size > 0)
    {
        Flight current = tempHeap.extractMin();

        string priorityName;
        string priorityColor;

        if (current.priority == 1)
        {
            priorityName = "CRITICAL";
            priorityColor = RED;
        }
        else if (current.priority == 2)
        {
            priorityName = "LOW FUEL";
            priorityColor = YELLOW;
        }
        else if (current.priority == 3)
        {
            priorityName = "NORMAL";
            priorityColor = GREEN;
        }
        else
        {
            priorityName = "LOW";
            priorityColor = CYAN;
        }

        cout << BOLD << "  " << setw(2) << position << ". " << RESET
             << CYAN << setw(12) << current.flightID << RESET << " | "
             << priorityColor << BOLD << setw(12) << priorityName << RESET << endl;

        position++;
    }

    cout << endl;
}
#ifndef AVLTREE_H
#define AVLTREE_H

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;

// ========== FLIGHT LOG STRUCTURE ==========
struct FlightLog
{
    string flightID;
    string origin;
    string destination;
    string arrivalTime;
    string result; // "LANDED" or "CRASHED"

    FlightLog() : flightID(""), origin(""), destination(""),
                  arrivalTime(""), result("") {}

    FlightLog(string id, string org, string dest, string time, string res)
        : flightID(id), origin(org), destination(dest),
          arrivalTime(time), result(res) {}
};

// ========== AVL TREE NODE ==========
struct AVLNode
{
    FlightLog logData;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(FlightLog log) : logData(log), left(NULL), right(NULL), height(1) {}
};

// ========== AVL TREE CLASS ==========
class AVLTree
{
private:
    AVLNode *root;

    // Helper functions
    int getHeight(AVLNode *node);
    int getBalance(AVLNode *node);

    AVLNode *rotateRight(AVLNode *node);
    AVLNode *rotateLeft(AVLNode *node);

    AVLNode *insertNode(AVLNode *node, FlightLog log);
    AVLNode *deleteNode(AVLNode *node, string flightID);
    AVLNode *findMin(AVLNode *node);

    void inOrderTraversal(AVLNode *node);
    AVLNode *searchNode(AVLNode *node, string flightID);
    void deleteTree(AVLNode *node);

public:
    // Constructor & Destructor
    AVLTree();
    ~AVLTree();

    // Insert flight log
    void insertLog(string flightID, string origin, string destination,
                   string time, string result);

    // Delete flight log
    bool deleteLog(string flightID);

    // Search flight log
    FlightLog *searchLog(string flightID);

    // Print all logs in chronological order
    void printAllLogs();

    // Get tree height
    int getTreeHeight();

    // Check if tree is empty
    bool isEmpty();
};

#endif // AVLTREE_H
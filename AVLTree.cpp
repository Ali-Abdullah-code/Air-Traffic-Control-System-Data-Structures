#include "AVLTree.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

using namespace std;

// ========== COLOR CODES ==========
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";
const string BOLD = "\033[1m";

// ========== CONSTRUCTOR ==========
AVLTree::AVLTree()
{
    root = NULL;
    cout << GREEN << "[SUCCESS] Flight Log system initialized" << RESET << endl;
}

// ========== DESTRUCTOR ==========
AVLTree::~AVLTree()
{
    deleteTree(root);
}

// ========== DELETE ENTIRE TREE ==========
void AVLTree::deleteTree(AVLNode *node)
{
    if (node == NULL)
        return;

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// ========== GET HEIGHT OF NODE ==========
int AVLTree::getHeight(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

// ========== GET BALANCE FACTOR ==========
int AVLTree::getBalance(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// ========== RIGHT ROTATION ==========
AVLNode *AVLTree::rotateRight(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
}

// ========== LEFT ROTATION ==========
AVLNode *AVLTree::rotateLeft(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

// ========== INSERT NODE RECURSIVELY ==========
AVLNode *AVLTree::insertNode(AVLNode *node, FlightLog log)
{
    // Base case: create new node
    if (node == NULL)
    {
        return new AVLNode(log);
    }

    // Insert based on timestamp comparison
    if (log.arrivalTime < node->logData.arrivalTime)
    {
        node->left = insertNode(node->left, log);
    }
    else if (log.arrivalTime > node->logData.arrivalTime)
    {
        node->right = insertNode(node->right, log);
    }
    else
    {
        // Duplicate timestamp, don't insert
        return node;
    }

    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // LL Case (Left-Left)
    if (balance > 1 && log.arrivalTime < node->left->logData.arrivalTime)
    {
        return rotateRight(node);
    }

    // RR Case (Right-Right)
    if (balance < -1 && log.arrivalTime > node->right->logData.arrivalTime)
    {
        return rotateLeft(node);
    }

    // LR Case (Left-Right)
    if (balance > 1 && log.arrivalTime > node->left->logData.arrivalTime)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // RL Case (Right-Left)
    if (balance < -1 && log.arrivalTime < node->right->logData.arrivalTime)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// ========== FIND MINIMUM NODE ==========
AVLNode *AVLTree::findMin(AVLNode *node)
{
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
    {
        node = node->left;
    }

    return node;
}

// ========== DELETE NODE RECURSIVELY ==========
AVLNode *AVLTree::deleteNode(AVLNode *node, string flightID)
{
    if (node == NULL)
        return NULL;

    // Find node to delete
    if (flightID < node->logData.flightID)
    {
        node->left = deleteNode(node->left, flightID);
    }
    else if (flightID > node->logData.flightID)
    {
        node->right = deleteNode(node->right, flightID);
    }
    else
    {
        // Node found

        // Case 1: Node has no children (leaf)
        if (node->left == NULL && node->right == NULL)
        {
            delete node;
            return NULL;
        }

        // Case 2: Node has only one child
        if (node->left == NULL)
        {
            AVLNode *temp = node->right;
            delete node;
            return temp;
        }

        if (node->right == NULL)
        {
            AVLNode *temp = node->left;
            delete node;
            return temp;
        }

        // Case 3: Node has two children
        AVLNode *minNode = findMin(node->right);
        node->logData = minNode->logData;
        node->right = deleteNode(node->right, minNode->logData.flightID);
    }

    if (node == NULL)
        return NULL;

    // Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // LL Case
    if (balance > 1 && getBalance(node->left) >= 0)
    {
        return rotateRight(node);
    }

    // LR Case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // RR Case
    if (balance < -1 && getBalance(node->right) <= 0)
    {
        return rotateLeft(node);
    }

    // RL Case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// ========== SEARCH NODE ==========
AVLNode *AVLTree::searchNode(AVLNode *node, string flightID)
{
    if (node == NULL)
        return NULL;

    if (flightID == node->logData.flightID)
    {
        return node;
    }

    if (flightID < node->logData.flightID)
    {
        return searchNode(node->left, flightID);
    }

    return searchNode(node->right, flightID);
}

// ========== IN-ORDER TRAVERSAL (Print logs chronologically) ==========
void AVLTree::inOrderTraversal(AVLNode *node)
{
    if (node == NULL)
        return;

    // Traverse left subtree
    inOrderTraversal(node->left);

    // Print current node
    cout << BOLD << "  +------------------------------------------+\n"
         << RESET;
    cout << "  | " << CYAN << "Flight ID: " << RESET << node->logData.flightID << endl;
    cout << "  | " << CYAN << "Origin: " << RESET << node->logData.origin << endl;
    cout << "  | " << CYAN << "Destination: " << RESET << node->logData.destination << endl;
    cout << "  | " << CYAN << "Arrival Time: " << RESET << node->logData.arrivalTime << endl;

    if (node->logData.result == "LANDED")
    {
        cout << "  | " << CYAN << "Result: " << RESET << GREEN << node->logData.result << RESET << endl;
    }
    else
    {
        cout << "  | " << CYAN << "Result: " << RESET << RED << node->logData.result << RESET << endl;
    }

    cout << BOLD << "  +------------------------------------------+\n"
         << RESET;

    // Traverse right subtree
    inOrderTraversal(node->right);
}

// ========== INSERT LOG ==========
void AVLTree::insertLog(string flightID, string origin, string destination,
                        string time, string result)
{

    FlightLog newLog(flightID, origin, destination, time, result);
    root = insertNode(root, newLog);

    cout << GREEN << "[LOG]" << RESET << " Flight " << CYAN << flightID
         << RESET << " recorded - " << result << endl;
}

// ========== DELETE LOG ==========
bool AVLTree::deleteLog(string flightID)
{
    AVLNode *node = searchNode(root, flightID);

    if (node == NULL)
    {
        cout << RED << "[ERROR]" << RESET << " Flight log not found" << endl;
        return false;
    }

    root = deleteNode(root, flightID);

    cout << GREEN << "[DELETE]" << RESET << " Flight log deleted" << endl;
    return true;
}

// ========== SEARCH LOG ==========
FlightLog *AVLTree::searchLog(string flightID)
{
    AVLNode *node = searchNode(root, flightID);

    if (node == NULL)
    {
        cout << RED << "[NOT FOUND]" << RESET << " Flight log not found" << endl;
        return NULL;
    }

    cout << GREEN << "[FOUND]" << RESET << " Flight log located" << endl;
    return &(node->logData);
}

// ========== PRINT ALL LOGS ==========
void AVLTree::printAllLogs()
{
    cout << BOLD << CYAN;
    cout << "\n+=============================================================+\n";
    cout << "|              " << YELLOW << "FLIGHT LOG HISTORY (CHRONOLOGICAL)" << CYAN << "        |\n";
    cout << "+=============================================================+\n";
    cout << RESET << endl;

    if (root == NULL)
    {
        cout << YELLOW << "  No flight logs available\n"
             << RESET << endl;
        return;
    }

    inOrderTraversal(root);

    cout << endl;
}

// ========== GET TREE HEIGHT ==========
int AVLTree::getTreeHeight()
{
    return getHeight(root);
}

// ========== CHECK IF EMPTY ==========
bool AVLTree::isEmpty()
{
    return root == NULL;
}
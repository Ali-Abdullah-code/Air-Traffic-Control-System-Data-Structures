#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <iostream>
#include <climits>

using namespace std;

const int MAX_NODES = 20;

// Node type: Airport or Waypoint
enum NodeType
{
    AIRPORT,
    WAYPOINT
};

// ========== SINGLE NODE (Vertex) ==========
struct Node
{
    int id;
    string name;
    int x, y;
    NodeType type;
    string occupiedBy;

    Node() : id(-1), name(""), x(0), y(0), type(WAYPOINT), occupiedBy("") {}

    Node(int id, string name, int x, int y, NodeType type)
        : id(id), name(name), x(x), y(y), type(type), occupiedBy("") {}
};

// ========== SINGLE EDGE (Connection between nodes) ==========
struct Edge
{
    int toNode; // Destination node ID
    int weight; // Distance in km

    Edge() : toNode(-1), weight(0) {}
    Edge(int to, int w) : toNode(to), weight(w) {}
};

// ========== LINKED LIST FOR EDGES ==========
struct EdgeNode
{
    Edge edge;
    EdgeNode *next;

    EdgeNode(Edge e) : edge(e), next(nullptr) {}
};

// ========== DIJKSTRA RESULT ==========
struct PathResult
{
    int distance[MAX_NODES];
    int parent[MAX_NODES];
};

// ========== GRAPH CLASS ==========
class Graph
{
private:
    Node nodes[MAX_NODES];      // All nodes
    EdgeNode *edges[MAX_NODES]; // Edges from each node
    int totalNodes;

public:
    // Constructor & Destructor
    Graph();
    ~Graph();

    // Add nodes (airports and waypoints)
    // CHANGED: x, y are now int (0-29, 0-19) for direct radar grid use
    void addNode(int id, string name, int x, int y, NodeType type);

    // Add edges (connections between nodes)
    // CHANGED: weight is now int
    void addEdge(int fromID, int toID, int weight);

    // Get node information
    Node *getNode(int nodeID);
    int getNodeID(string nodeName);
    int getTotalNodes();

    // Check if node is occupied
    bool isOccupied(int nodeID);
    void occupyNode(int nodeID, string flightID);
    void freeNode(int nodeID);

    // Dijkstra's algorithm for shortest path
    PathResult dijkstra(int startNodeID);

    // Get path from start to end
    void getPath(int startID, int endID, int path[], int &pathLength);

    // Find nearest airport from current position
    int findNearestAirport(int currentNodeID);

    // Display functions
    void displayAllNodes();
    void displayPath(int path[], int length);
    void displayGraph();
};

#endif // GRAPH_H
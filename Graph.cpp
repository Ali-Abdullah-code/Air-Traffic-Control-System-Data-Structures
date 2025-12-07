#include "Graph.h"
#include <cmath>
#include <climits>

// ========== CONSTRUCTOR ==========
Graph::Graph()
{
    totalNodes = 0;

    // Initialize all edge lists to NULL
    for (int i = 0; i < MAX_NODES; i++)
    {
        edges[i] = NULL;
    }
}

// ========== DESTRUCTOR ==========
Graph::~Graph()
{
    // Delete all edge lists
    for (int i = 0; i < MAX_NODES; i++)
    {
        EdgeNode *current = edges[i];
        while (current != NULL)
        {
            EdgeNode *temp = current;
            current = current->next;
            delete temp;
        }
    }
}

// ========== ADD NODE (Airport or Waypoint) ==========
void Graph::addNode(int id, string name, int x, int y, NodeType type)
{
    if (totalNodes >= MAX_NODES)
    {
        cout << "Cannot add more nodes. Maximum limit reached." << endl;
        return;
    }

    // Validate coordinates for radar grid (0-29, 0-19)
    if (x < 0 || x >= 30 || y < 0 || y >= 20)
    {
        cout << "Error: Coordinates out of radar grid bounds (0-29, 0-19)" << endl;
        return;
    }

    nodes[totalNodes].id = id;
    nodes[totalNodes].name = name;
    nodes[totalNodes].x = x;
    nodes[totalNodes].y = y;
    nodes[totalNodes].type = type;
    nodes[totalNodes].occupiedBy = "";

    totalNodes++;
    cout << "Node added: " << name << " (ID: " << id << ") at (" << x << ", " << y << ")" << endl;
}

// ========== ADD EDGE (Connection between nodes) ==========
void Graph::addEdge(int fromID, int toID, int weight)
{
    // Find the index of fromID in nodes array
    int fromIndex = -1;
    int toIndex = -1;

    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id == fromID)
            fromIndex = i;
        if (nodes[i].id == toID)
            toIndex = i;
    }

    if (fromIndex == -1 || toIndex == -1)
    {
        cout << "Error: One or both nodes do not exist." << endl;
        return;
    }

    // Create new edge and add to linked list
    Edge newEdge(toID, weight);

    EdgeNode *newEdgeNode = new EdgeNode(newEdge);
    newEdgeNode->next = edges[fromIndex];
    edges[fromIndex] = newEdgeNode;

    cout << "Edge added: " << nodes[fromIndex].name << " -> "
         << nodes[toIndex].name << " (Weight: " << weight << " km)" << endl;
}

// ========== GET NODE BY ID ==========
Node *Graph::getNode(int nodeID)
{
    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id == nodeID)
        {
            return &nodes[i];
        }
    }
    return NULL;
}

// ========== GET NODE ID BY NAME ==========
int Graph::getNodeID(string nodeName)
{
    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].name == nodeName)
        {
            return nodes[i].id;
        }
    }
    return -1;
}

// ========== GET TOTAL NUMBER OF NODES ==========
int Graph::getTotalNodes()
{
    return totalNodes;
}

// ========== CHECK IF NODE IS OCCUPIED ==========
bool Graph::isOccupied(int nodeID)
{
    Node *node = getNode(nodeID);
    if (node != NULL)
    {
        return node->occupiedBy != "";
    }
    return false;
}

// ========== OCCUPY NODE (Plane lands here) ==========
void Graph::occupyNode(int nodeID, string flightID)
{
    Node *node = getNode(nodeID);
    if (node != NULL)
    {
        node->occupiedBy = flightID;
        cout << "Node " << node->name << " occupied by flight " << flightID << endl;
    }
}

// ========== FREE NODE (Plane leaves) ==========
void Graph::freeNode(int nodeID)
{
    Node *node = getNode(nodeID);
    if (node != NULL)
    {
        node->occupiedBy = "";
        cout << "Node " << node->name << " is now free." << endl;
    }
}

// ========== DIJKSTRA'S ALGORITHM ==========
PathResult Graph::dijkstra(int startNodeID)
{
    PathResult result;
    bool visited[MAX_NODES] = {false};

    // Initialize distances to INF, start to 0
    for (int i = 0; i < MAX_NODES; i++)
    {
        result.distance[i] = INT_MAX;
        result.parent[i] = -1;
    }

    // Find starting node index
    int startIndex = -1;
    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id == startNodeID)
        {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1)
    {
        cout << "Start node not found." << endl;
        return result;
    }

    result.distance[startIndex] = 0;

    // Relax edges totalNodes - 1 times
    for (int count = 0; count < totalNodes - 1; count++)
    {
        // Find unvisited node with minimum distance
        int minIndex = -1;
        int minDist = INT_MAX;

        for (int i = 0; i < totalNodes; i++)
        {
            if (!visited[i] && result.distance[i] < minDist)
            {
                minDist = result.distance[i];
                minIndex = i;
            }
        }

        if (minIndex == -1)
            break;

        visited[minIndex] = true;

        // Check all edges from this node
        EdgeNode *current = edges[minIndex];
        while (current != NULL)
        {
            // Find index of destination node
            int toIndex = -1;
            for (int i = 0; i < totalNodes; i++)
            {
                if (nodes[i].id == current->edge.toNode)
                {
                    toIndex = i;
                    break;
                }
            }

            if (toIndex != -1 && !visited[toIndex])
            {
                int newDist = result.distance[minIndex] + current->edge.weight;

                if (newDist < result.distance[toIndex])
                {
                    result.distance[toIndex] = newDist;
                    result.parent[toIndex] = minIndex;
                }
            }

            current = current->next;
        }
    }

    return result;
}

// ========== GET PATH FROM START TO END ==========
void Graph::getPath(int startID, int endID, int path[], int &pathLength)
{
    pathLength = 0;

    // Find indices
    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id == startID)
            startIndex = i;
        if (nodes[i].id == endID)
            endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1)
    {
        cout << "Invalid start or end node." << endl;
        return;
    }

    // Run Dijkstra from start
    PathResult result = dijkstra(startID);

    if (result.distance[endIndex] == INT_MAX)
    {
        cout << "No path exists from " << nodes[startIndex].name
             << " to " << nodes[endIndex].name << endl;
        return;
    }

    // Reconstruct path using parent array
    int pathArray[MAX_NODES];
    int tempLength = 0;
    int current = endIndex;

    while (current != -1)
    {
        pathArray[tempLength++] = nodes[current].id;
        current = result.parent[current];
    }

    // Reverse to get correct order
    for (int i = tempLength - 1; i >= 0; i--)
    {
        path[pathLength++] = pathArray[i];
    }
}

// ========== FIND NEAREST AIRPORT ==========
int Graph::findNearestAirport(int currentNodeID)
{
    // Find current node index
    int currentIndex = -1;
    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id == currentNodeID)
        {
            currentIndex = i;
            break;
        }
    }

    if (currentIndex == -1)
    {
        return -1;
    }

    // Run Dijkstra from current position
    PathResult result = dijkstra(currentNodeID);

    // Find nearest airport
    int nearestAirport = -1;
    int minDistance = INT_MAX;

    for (int i = 0; i < totalNodes; i++)
    {
        // Skip the current node itself and check if it's an airport
        if (i != currentIndex && nodes[i].type == AIRPORT && result.distance[i] < minDistance)
        {
            minDistance = result.distance[i];
            nearestAirport = nodes[i].id;
        }
    }

    // If no other airport found and current location is an airport, return current
    if (nearestAirport == -1 && nodes[currentIndex].type == AIRPORT)
    {
        nearestAirport = nodes[currentIndex].id;
    }

    return nearestAirport;
}

// ========== DISPLAY ALL NODES ==========
void Graph::displayAllNodes()
{
    cout << "\n+---------- ALL NODES IN AIRSPACE ----------+" << endl;
    cout << "Total Nodes: " << totalNodes << endl;
    cout << "+-------------------------------------------+" << endl;

    for (int i = 0; i < totalNodes; i++)
    {
        cout << "ID: " << nodes[i].id
             << " | Name: " << nodes[i].name
             << " | Type: " << (nodes[i].type == AIRPORT ? "AIRPORT" : "WAYPOINT")
             << " | Position: (" << nodes[i].x << ", " << nodes[i].y << ")"
             << " | Occupied: " << (nodes[i].occupiedBy == "" ? "NO" : nodes[i].occupiedBy)
             << endl;
    }

    cout << "+-------------------------------------------+\n"
         << endl;
}

// ========== DISPLAY PATH ==========
void Graph::displayPath(int path[], int length)
{
    if (length == 0)
    {
        cout << "No path to display." << endl;
        return;
    }

    cout << "\n+---------- FLIGHT PATH ----------+" << endl;
    for (int i = 0; i < length; i++)
    {
        Node *node = getNode(path[i]);
        if (node != NULL)
        {
            cout << node->name;
            if (i < length - 1)
            {
                cout << " -> ";
            }
        }
    }
    cout << "\n+--------------------------------+\n"
         << endl;
}

// ========== DISPLAY COMPLETE GRAPH ==========
void Graph::displayGraph()
{
    cout << "\n+========== AIRSPACE NETWORK ==========+" << endl;
    cout << "Total Nodes: " << totalNodes << "\n"
         << endl;

    for (int i = 0; i < totalNodes; i++)
    {
        if (nodes[i].id != -1)
        {
            string typeStr = (nodes[i].type == AIRPORT) ? "AIRPORT" : "WAYPOINT";
            cout << "[" << nodes[i].id << "] " << nodes[i].name
                 << " (" << typeStr << ") at (" << nodes[i].x << ", " << nodes[i].y << ")";

            if (nodes[i].occupiedBy != "")
            {
                cout << " - OCCUPIED by " << nodes[i].occupiedBy;
            }
            cout << endl;

            // Display edges from this node
            EdgeNode *current = edges[nodes[i].id];
            if (current != NULL)
            {
                cout << "  Connections: ";
                while (current != NULL)
                {
                    Node *destNode = getNode(current->edge.toNode);
                    if (destNode != NULL)
                    {
                        cout << destNode->name << " (" << current->edge.weight << " km)";
                        if (current->next != NULL)
                        {
                            cout << ", ";
                        }
                    }
                    current = current->next;
                }
                cout << endl;
            }
            cout << endl;
        }
    }
    cout << "+======================================+\n"
         << endl;
}
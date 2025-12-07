# Air Traffic Control System Data Structures

A comprehensive C++ implementation of an Air Traffic Control (ATC) system demonstrating advanced data structures and algorithms for airspace management, flight routing, and system operations.

## Features

### Core Functionality
- **Airspace Visualization**: Interactive display of airports, waypoints, and flight paths
- **Flight Management**: Add, track, and manage aircraft with real-time positioning
- **Route Planning**: Dijkstra's algorithm for finding optimal safe routes between airports
- **Emergency Routing**: Automatic nearest airport detection for emergency landings
- **Fuel Management**: Manual fuel level control and monitoring
- **Flight Logging**: Chronological flight event logging using AVL Tree for efficient storage and retrieval

### Data Structures Implemented
- **Graph**: Adjacency list representation for airspace network with Dijkstra's shortest path algorithm
- **AVL Tree**: Self-balancing binary search tree for flight log management (used in Search & Reporting menu)
- **HashTable**: Fast lookup data structure for aircraft registry using separate chaining collision resolution (integrated with Flight Manager for aircraft record management)
- **MinHeap**: Priority queue data structure for landing sequence management (implemented and available for future expansion)

### System Management
- **Dynamic Airspace Control**: Add/remove airports, waypoints, and flight connections
- **Persistence**: Save and load system state to/from files
- **Real-time Monitoring**: Live airspace status and flight tracking

## Project Structure

```
Air-Traffic-Control-System-Data-Structures/
├── main.cpp              # Main application entry point and menu system
├── Graph.cpp             # Airspace network and routing algorithms
├── Graph.h               # Graph class declarations (includes Node/Edge structures)
├── FlightManager.cpp     # Flight lifecycle and system management
├── FlightManager.h       # FlightManager class declarations
├── AVLTree.cpp           # AVL Tree implementation for flight logs
├── AVLTree.h             # AVL Tree class declarations
├── HashTable.cpp         # Hash table for registry management
├── HashTable.h           # Hash table class declarations
├── MinHeap.cpp           # Min-heap for priority operations
├── MinHeap.h             # Min-heap class declarations
├── Radar.cpp             # Airspace visualization and display
├── Radar.h               # Radar class declarations
└── README.md             # Project documentation
```

## Prerequisites

- **C++ Compiler**: GCC (g++) or compatible C++11+ compiler
- **Operating System**: Windows, Linux, or macOS
- **Build Tools**: Make (optional, for automated builds)

## Installation & Setup

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Ali-Abdullah-code/Air-Traffic-Control-System-Data-Structures.git
   cd Air-Traffic-Control-System-Data-Structures
   ```

2. **Compile the Project**
   ```bash
   g++ AVLTree.cpp FlightManager.cpp Graph.cpp HashTable.cpp main.cpp MinHeap.cpp Radar.cpp -o atc_system.exe
   ```

3. **Run the Application**
   ```bash
   ./atc_system.exe
   ```

## Usage

### Main Menu Navigation

1. **Airspace Visualizer** - Display current airspace layout with airports, waypoints, and active flights
2. **Flight Control** - Manage aircraft operations (add flights, declare emergencies, move flights, update fuel, view active flights, land aircraft)
3. **Search & Reporting** - Search flights, print flight logs, find safe routes using Dijkstra's algorithm
4. **System Management** - Dynamic airspace control (add airports/waypoints, add flight corridors, view network, save/load data, system status)
5. **Exit** - Close the application

### Key Operations

- **Route Finding**: Navigate to Search & Reporting → Find Safe Route to use Dijkstra's algorithm for optimal path calculation between locations
- **Emergency Handling**: Flight Control → Declare Emergency to change flight priority levels
- **Dynamic Airspace**: System Management allows adding new airports, waypoints, and flight corridors
- **Data Persistence**: System Management → Save/Load System Data for complete airspace configuration
- **Flight Logging**: Search & Reporting → Print Flight Log displays chronological events using AVL Tree

## Algorithms & Data Structures

### Dijkstra's Algorithm
- **Location**: Implemented in `Graph::dijkstra()` method
- **Usage**: Calculates shortest paths between locations considering distance weights
- **Access**: Search & Reporting Menu → Option 3 (Find Safe Route)

### AVL Tree
- **Purpose**: Maintains balanced flight logs for O(log n) insertion and retrieval
- **Operations**: Insert, search, and display chronological flight events

### Hash Table
- **Purpose**: Fast O(1) average-case lookup for aircraft registry
- **Implementation**: Separate chaining for collision resolution

### Min Heap
- **Purpose**: Priority queue for landing sequence management
- **Operations**: Insert aircraft with priority, extract minimum for landing order

## Technical Details

- **Language**: C++11+
- **Memory Management**: Manual memory management with proper cleanup
- **File I/O**: Binary serialization for system state persistence
- **Error Handling**: Input validation and graceful error recovery
- **Performance**: Optimized algorithms for real-time airspace operations

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Ali Abdullah - [GitHub Profile](https://github.com/Ali-Abdullah-code)

## Acknowledgments

- Implementation demonstrates practical application of fundamental data structures
- Dijkstra's algorithm visualization for educational purposes
- Real-world air traffic control concepts adapted for educational use
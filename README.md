# Blood Circulation Graph Simulator

This is our Graph Theory project for simulating a simplified human blood circulatory system using C++.

## What we are doing

- Heart chambers, lungs and main organs are represented as graph vertices.
- Blood vessels are represented as directed weighted edges.
- Distance and flow rate are stored as edge values.
- The graph is stored using an adjacency list.
- BFS, DFS and Dijkstra are implemented in C++.
- Red Blood Cells, White Blood Cells and Platelets move through the graph.
- Flow rate is used when choosing a path at a branch.
- The program keeps track of organ visits, movements and probabilities.
- A visual version is also available to show the circulation graph and moving blood cells.

## How to run on Windows

### Terminal program

Compile the C++ files using a C++17 compiler:

```bash
g++ -std=c++17 src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
```

Then run:

```bash
circulation_sim.exe
```

This opens the terminal menu where we can view the graph connections, run BFS and DFS, find shortest paths, simulate the blood cells and export the graph.

### Visual program

The visual version is built using CMake.

```bash
cmake -S . -B build
cmake --build build
```

Then run:

```bash
build\circulation_visual.exe
```

This opens the graphical circulation view.

## Current graph

The graph contains:

- Right Atrium
- Right Ventricle
- Left Atrium
- Left Ventricle
- Lungs
- Aorta
- Venae Cavae
- Brain
- Heart Muscle
- Kidneys
- Liver
- Digestive Tract
- Spleen
- Other Tissues

The distance and flow values used in the program are simplified assumptions for the simulation.

## More details

For a simple explanation of how blood circulation works, how we mapped it into a graph and what each menu option does, see:

[Project Explanation](docs/PROJECT_EXPLANATION.md)

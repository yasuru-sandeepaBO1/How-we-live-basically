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
- The graph can be exported as a Graphviz `.dot` file.

## How to run

Compile the C++ files using a C++17 compiler:

```bash
g++ -std=c++17 src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
```

On macOS/Linux:

```bash
./circulation_sim
```

On Windows:

```bash
circulation_sim.exe
```

The terminal menu can be used to view graph connections, run BFS and DFS, find the shortest route, run the blood-cell profiling options and export the Graphviz file.

## Graphviz map

Choose the Graphviz export option from the terminal menu to create:

```text
circulation.dot
```

If Graphviz is installed, the file can be converted into an image.

For example:

```bash
dot -Tpng circulation.dot -o circulation.png
```

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

# Blood Circulation Graph Simulator

This is our Graph Theory project for simulating a simplified human blood circulatory system using C++.

## What we are doing

- Represent the four heart chambers, lungs and important organs as graph vertices.
- Represent blood vessels as directed weighted edges.
- Store distance and flow rate for each connection.
- Implement BFS and DFS ourselves using an adjacency list.
- Use Dijkstra's algorithm to find the shortest route between two locations.
- Represent Red Blood Cells, White Blood Cells and Platelets using separate C++ structures and simulate them moving through the graph.
- Keep track of how many times cells visit each organ and calculate visit probabilities.
- Use different lifespans for the three cell types.
- Export the graph as a Graphviz `.dot` file so the circulation network can be visualized.

## Main idea

The circulatory system is treated as a directed weighted graph. A blood cell starts at a location and moves through connected blood vessels. At branches, the flow-rate values are used as weights when selecting the next path.

## Build

Requires a C++17 compiler.

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
./circulation_sim
```

Graph visualization is exported to `circulation.dot`. If Graphviz is installed, it can be converted to an image with:

```bash
dot -Tpng circulation.dot -o circulation.png
```

## Project status

First working version includes the graph, BFS, DFS, shortest-path calculation, cell simulation, profiling and Graphviz export. We can improve the visual simulation and biological assumptions after testing the core program.

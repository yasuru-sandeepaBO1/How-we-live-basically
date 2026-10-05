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

### 1. Open the project folder

If the project is in your Downloads folder:

```bash
cd ~/Downloads/How-we-live-basically
```

If your project is stored somewhere else, change the path accordingly.

### 2. Compile the program

Compile the C++ source files using a C++17 compiler:

```bash
g++ -std=c++17 src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
```

### 3. Run the program

On macOS/Linux:

```bash
./circulation_sim
```

On Windows:

```bash
circulation_sim.exe
```

The terminal menu can be used to:

- View graph connections
- Run BFS
- Run DFS
- Find the shortest route using Dijkstra
- Run Red Blood Cell profiling
- Run White Blood Cell profiling
- Run Platelet profiling
- Run all three profiling options
- Export the Graphviz file

## Graphviz map

The program creates a Graphviz file named:

```text
circulation.dot
```

You can also choose option **9** from the terminal menu to export it again.

### Install Graphviz on macOS

If Graphviz is not installed:

```bash
brew install graphviz
```

### Convert the DOT file to PNG

Run:

```bash
dot -Tpng circulation.dot -o circulation.png
```

This creates:

```text
circulation.png
```

### Open the generated graph on macOS

```bash
open circulation.png
```

### Quick command sequence for macOS

```bash
cd ~/Downloads/How-we-live-basically
g++ -std=c++17 src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
./circulation_sim
dot -Tpng circulation.dot -o circulation.png
open circulation.png
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

# Blood Circulation Graph Simulator

This is our Graph Theory project for simulating a simplified human blood circulatory system using C++.

## What we are doing

- Heart chambers, lungs and main organs are vertices.
- Blood vessels are directed weighted edges.
- Edge weights store distance and flow rate.
- The graph is stored using an adjacency list.
- BFS, DFS and Dijkstra are implemented in C++.
- RBC, WBC and Platelets move through the graph.
- Flow rate is used when choosing a path at a branch.
- Organ visits, movement counts and probabilities can be profiled.
- The visual program shows oxygen-rich, oxygen-poor and portal routes with moving blood cells.

## Visual simulator

The main visual version uses SFML only for drawing the window. The graph and algorithms are still our own C++ code.

The screen contains the circulation graph on the left and the live controls/statistics on the right. Blood cells move along the directed edges while the simulation is running.

### Build on macOS / Linux

You need Git, CMake and a C++17 compiler. SFML 2.6.1 is downloaded automatically by CMake when the project is configured.

```bash
git clone https://github.com/yasuru-sandeepaBO1/How-we-live-basically.git
cd How-we-live-basically
cmake -S . -B build
cmake --build build -j
./build/circulation_visual
```

On macOS, if the compiler tools are missing:

```bash
xcode-select --install
```

If CMake is missing and Homebrew is installed:

```bash
brew install cmake
```

### Visual controls

- **Pause / Resume** button: stops or continues the moving cells.
- **Speed** button: cycles through 1x, 2x, 5x, 10x and 0.5x.
- **Space bar**: pause/resume shortcut.

## Terminal version

The terminal version is still included because it is easier to demonstrate BFS, DFS and Dijkstra step by step.

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/main.cpp src/CirculatoryGraph.cpp src/Simulation.cpp -o circulation_sim
./circulation_sim
```

It can also export `circulation.dot` for Graphviz.

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

The numbers used for distance and flow are simplified assumptions for the graph simulation, not medical measurements for a real patient.

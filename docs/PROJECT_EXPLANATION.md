# Project Explanation

## 1. First, what is the blood circulation system?

Before looking at the program, it is useful to understand the basic idea of blood circulation.

The circulatory system moves blood around the body. The main parts we are interested in are the heart, lungs, blood vessels and important organs.

The heart has four chambers:

- Right Atrium
- Right Ventricle
- Left Atrium
- Left Ventricle

The right side of the heart mainly receives oxygen-poor blood from the body and sends it to the lungs.

The left side of the heart receives oxygen-rich blood from the lungs and sends it to the rest of the body.

A simplified circulation path is:

```text
Body
  -> Right Atrium
  -> Right Ventricle
  -> Lungs
  -> Left Atrium
  -> Left Ventricle
  -> Aorta
  -> Body organs
  -> Venae Cavae
  -> Right Atrium
```

So the blood keeps moving in a cycle.

### Lungs

The lungs are where blood gets oxygen.

Blood going from the right ventricle to the lungs is oxygen-poor. After passing through the lungs, it becomes oxygen-rich and returns to the left atrium.

### Aorta

The aorta is the main artery that carries oxygen-rich blood away from the left ventricle to the body.

From the aorta, blood can go to places such as:

- Brain
- Kidneys
- Liver
- Heart Muscle
- Digestive Tract
- Spleen
- Other Tissues

### Venae Cavae

The venae cavae return oxygen-poor blood from the body back to the right atrium.

### Liver and digestive system

Blood from the digestive tract can first travel to the liver before returning to the main circulation. In our graph this is shown as a portal route.

---

## 2. Blood cells used in the project

The assignment asks us to represent three types of blood cells separately.

### Red Blood Cells

Red Blood Cells are mainly responsible for carrying oxygen.

In our program they travel through the circulation graph and we keep track of things such as:

- number of movements
- organs visited
- distance travelled
- number of circulation cycles
- lifespan used by the simulation

### White Blood Cells

White Blood Cells are related to the immune system.

They are represented separately from Red Blood Cells and can also travel through the graph.

### Platelets

Platelets help with blood clotting.

They are also represented separately and move through the circulation graph.

For the program, the lifespans and time steps are simplified because this is a graph simulation, not a medical simulator.

---

## 3. How this becomes a Graph Theory problem

We represent the circulatory system as a directed weighted graph.

### Vertices

The heart chambers and organs are vertices.

Examples:

```text
Right Atrium
Right Ventricle
Lungs
Left Atrium
Left Ventricle
Brain
Kidneys
Liver
```

### Edges

Blood vessels are represented as directed edges.

For example:

```text
Right Atrium -> Right Ventricle
Right Ventricle -> Lungs
Lungs -> Left Atrium
```

The arrow shows the direction of blood flow.

### Weights

Each edge can store values such as:

- distance
- flow rate

Because of this, the circulation network is a directed weighted graph.

### Adjacency list

The graph is stored using an adjacency list.

Each vertex keeps a list of the vertices that can be reached from it.

This is the main graph data structure used by BFS, DFS, Dijkstra and the blood-cell profiling.

---

## 4. What we can do from the terminal program

When the C++ program is run, it shows a menu.

| Option | What it does |
|---|---|
| View graph connections | Shows each vertex and the outgoing connections with distance and flow-rate values. |
| BFS traversal | Runs Breadth-First Search from a selected starting vertex and prints the visiting order. |
| DFS traversal | Runs Depth-First Search from a selected starting vertex and prints the visiting order. |
| Shortest route (Dijkstra) | Finds the shortest path between two selected vertices using distance as the weight. |
| Simulate Red Blood Cells | Runs the Red Blood Cell profiling through the circulation graph. |
| Simulate White Blood Cells | Runs the White Blood Cell profiling through the circulation graph. |
| Simulate Platelets | Runs the Platelet profiling through the circulation graph. |
| Run all three simulations | Runs all three blood-cell profiling options one after another. |
| Export Graphviz file | Creates a `circulation.dot` file that can be used to draw the graph using Graphviz. |
| Exit | Closes the program. |

---

## 5. BFS in this project

BFS means Breadth-First Search.

It starts from one vertex and first visits the directly connected vertices before going deeper.

A queue is used for BFS.

---

## 6. DFS in this project

DFS means Depth-First Search.

Instead of checking all nearby vertices first, DFS follows one path as far as possible before coming back.

A visited list is important because the circulatory system contains cycles.

Without marking visited vertices, the traversal could keep going around the circulation loop.

---

## 7. Dijkstra in this project

Dijkstra's algorithm is used because our graph has weights.

For this project, distance is used as the weight for the shortest-path calculation.

The user selects a start vertex and a destination vertex. The program then prints the shortest directed route and its total distance.

---

## 8. How the blood-cell profiling works

A blood cell starts at a vertex in the graph and follows one of the outgoing edges.

If there are several possible routes, the flow-rate values are used as weights when selecting the next path.

So a route with a larger flow value has a higher chance of being selected.

While a cell moves, the program records information such as:

- current position
- movement count
- distance travelled
- organ visits
- completed circulation cycles

At the end, the program can calculate how often different organs were visited.

A simple visit probability is calculated using:

```text
organ visits / total organ visits
```

---

## 9. Graphviz map

The terminal program can export the circulation graph as:

```text
circulation.dot
```

This file contains the same graph vertices and directed edges used by the program.

If Graphviz is installed, the file can be converted to an image such as PNG.

---

## 10. Basic project structure

```text
src/
  main.cpp
  CirculatoryGraph.h
  CirculatoryGraph.cpp
  Simulation.h
  Simulation.cpp
  BloodCells.h
```

`CirculatoryGraph` handles the graph and graph algorithms.

`Simulation` handles blood-cell movement and profiling.

`main.cpp` contains the terminal menu.

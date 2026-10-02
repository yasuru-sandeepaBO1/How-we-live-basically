#include "CirculatoryGraph.h"
#include "Visualization.h"

#include <exception>
#include <iostream>

int main() {
    try {
        CirculatoryGraph graph = buildDefaultCirculatoryGraph();
        Visualization visualizer(graph);
        return visualizer.run();
    } catch (const std::exception& e) {
        std::cerr << "Could not start visual simulator: " << e.what() << '\n';
        return 1;
    }
}

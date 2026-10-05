#include "CirculatoryGraph.h"
#include "Simulation.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {
void printTraversal(const CirculatoryGraph& graph, const std::vector<int>& order) {
    for (std::size_t i = 0; i < order.size(); ++i) {
        std::cout << graph.vertex(order[i]).name;
        if (i + 1 < order.size()) std::cout << " -> ";
    }
    std::cout << '\n';
}

void showLocations(const CirculatoryGraph& graph) {
    for (int i = 0; i < graph.vertexCount(); ++i)
        std::cout << "  " << i << ". " << graph.vertex(i).name << '\n';
}

void printGap(int lines = 5) {
    for (int i = 0; i < lines; ++i) std::cout << '\n';
}

int readVertex(const CirculatoryGraph& graph, const std::string& prompt) {
    int index = -1;
    std::cout << prompt;
    std::cin >> index;
    if (!std::cin || index < 0 || index >= graph.vertexCount()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return index;
}

void runCellSimulation(Simulation& simulation, CellType type) {
    int count = 100;
    int steps = 200;

    std::cout << "Number of cells to simulate [default 100]: ";
    if (!(std::cin >> count) || count <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        count = 100;
    }
    count = std::min(count, 10000);

    std::cout << "Maximum steps per cell [default 200]: ";
    if (!(std::cin >> steps) || steps <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        steps = 200;
    }
    steps = std::min(steps, 100000);

    auto profiles = simulation.run(type, count, steps);
    simulation.printSummary(type, profiles);
}
}

int main() {
    CirculatoryGraph graph = buildDefaultCirculatoryGraph();
    Simulation simulation(graph);

    try {
        graph.exportDot("circulation.dot");
    } catch (const std::exception& e) {
        std::cerr << "Warning: " << e.what() << '\n';
    }

    while (true) {
        std::cout << "\nHuman Blood Circulation Graph Simulator\n\n";
        std::cout << "1. View graph connections\n";
        std::cout << "2. BFS traversal\n";
        std::cout << "3. DFS traversal\n";
        std::cout << "4. Shortest route (Dijkstra)\n";
        std::cout << "5. Simulate Red Blood Cells\n";
        std::cout << "6. Simulate White Blood Cells\n";
        std::cout << "7. Simulate Platelets\n";
        std::cout << "8. Run all three simulations\n";
        std::cout << "9. Export Graphviz file\n";
        std::cout << "0. Exit\n\n";
        std::cout << "Choose: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printGap();
            std::cout << "Invalid input. Please enter a menu number.\n";
            printGap();
            continue;
        }

        if (choice == 0) {
            printGap(3);
            std::cout << "Simulation closed.\n";
            break;
        }

        printGap();

        if (choice == 1) {
            graph.printGraph();

        } else if (choice == 2 || choice == 3) {
            std::cout << (choice == 2 ? "--- BFS Traversal ---\n\n" : "--- DFS Traversal ---\n\n");
            showLocations(graph);

            int start = readVertex(graph, "\nStart vertex number: ");
            if (start < 0) {
                std::cout << "\nInvalid vertex.\n";
                printGap();
                continue;
            }

            auto order = (choice == 2) ? graph.bfs(start) : graph.dfs(start);
            std::cout << "\n" << (choice == 2 ? "BFS: " : "DFS: ");
            printTraversal(graph, order);

        } else if (choice == 4) {
            std::cout << "--- Shortest Route ---\n\n";
            showLocations(graph);

            int start = readVertex(graph, "\nStart vertex number: ");
            int end = readVertex(graph, "Destination vertex number: ");

            if (start < 0 || end < 0) {
                std::cout << "\nInvalid vertex.\n";
                printGap();
                continue;
            }

            auto [distance, path] = graph.dijkstra(start, end);

            if (path.empty()) {
                std::cout << "\nNo directed path found.\n";
            } else {
                std::cout << "\nShortest path: ";
                printTraversal(graph, path);
                std::cout << "Distance: " << std::fixed << std::setprecision(2)
                          << distance << " m\n";
            }

        } else if (choice == 5) {
            std::cout << "--- Red Blood Cell Simulation ---\n\n";
            runCellSimulation(simulation, CellType::RedBloodCell);

        } else if (choice == 6) {
            std::cout << "--- White Blood Cell Simulation ---\n\n";
            runCellSimulation(simulation, CellType::WhiteBloodCell);

        } else if (choice == 7) {
            std::cout << "--- Platelet Simulation ---\n\n";
            runCellSimulation(simulation, CellType::Platelet);

        } else if (choice == 8) {
            std::cout << "--- All Blood Cell Simulations ---\n";
            auto rbcs = simulation.run(CellType::RedBloodCell, 100, 200);
            auto wbcs = simulation.run(CellType::WhiteBloodCell, 100, 200);
            auto platelets = simulation.run(CellType::Platelet, 100, 200);

            simulation.printSummary(CellType::RedBloodCell, rbcs);
            simulation.printSummary(CellType::WhiteBloodCell, wbcs);
            simulation.printSummary(CellType::Platelet, platelets);

        } else if (choice == 9) {
            std::cout << "--- Graphviz Export ---\n\n";

            try {
                graph.exportDot("circulation.dot");
                std::cout << "Created circulation.dot\n";
            } catch (const std::exception& e) {
                std::cout << e.what() << '\n';
            }

        } else {
            std::cout << "Unknown option. Please choose a number from 0 to 9.\n";
        }

        printGap();
    }

    return 0;
}

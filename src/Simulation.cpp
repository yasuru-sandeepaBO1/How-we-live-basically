#include "Simulation.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

Simulation::Simulation(const CirculatoryGraph& graph)
    : graph_(graph), rng_(std::random_device{}()) {}

std::string Simulation::cellTypeName(CellType type) {
    switch (type) {
        case CellType::RedBloodCell: return "Red Blood Cell";
        case CellType::WhiteBloodCell: return "White Blood Cell";
        case CellType::Platelet: return "Platelet";
    }

    return "Unknown";
}

// Returns the simplified lifespan used for each cell type in this model.
double Simulation::lifespanFor(CellType type) const {
    switch (type) {
        case CellType::RedBloodCell: return 120.0;
        case CellType::WhiteBloodCell: return 20.0;
        case CellType::Platelet: return 9.0;
    }

    return 1.0;
}

// Each graph movement increases the simulated age by this amount.
// These values are assumptions used to make the profiling finite and easy to compare.
double Simulation::daysPerStep(CellType type) const {
    switch (type) {
        case CellType::RedBloodCell: return 2.0;
        case CellType::WhiteBloodCell: return 1.0;
        case CellType::Platelet: return 0.5;
    }

    return 1.0;
}

// -------------------- Flow-based path selection --------------------
// When a vertex has several outgoing edges, the edge flow rates are used
// as weights. A higher-flow edge therefore has a higher chance of being selected.
int Simulation::chooseNextVertex(int current, double& edgeDistance) {
    const auto& edges = graph_.neighbors(current);

    if (edges.empty()) {
        edgeDistance = 0.0;
        return current;
    }

    std::vector<double> weights;

    for (const auto& edge : edges) {
        weights.push_back(std::max(0.001, edge.flowRateLpm));
    }

    std::discrete_distribution<int> distribution(weights.begin(), weights.end());
    const auto& selected = edges[distribution(rng_)];

    edgeDistance = selected.distanceMeters;
    return selected.to;
}

// -------------------- Blood-cell traversal --------------------
// Creates the requested number of cells and moves each one through the graph.
// A cell stops when it reaches the maximum requested steps or its simulated lifespan.
std::vector<CellProfile> Simulation::run(CellType type, int cellCount, int maxStepsPerCell) {
    std::vector<CellProfile> profiles;
    profiles.reserve(cellCount);

    // All cells begin at the Right Atrium.
    const int start = graph_.indexOf("Right Atrium");

    // Reaching the Left Ventricle is used as the cycle marker in this model.
    const int cycleMarker = graph_.indexOf("Left Ventricle");

    for (int id = 1; id <= cellCount; ++id) {
        CellProfile cell;

        cell.id = id;
        cell.type = type;
        cell.currentVertex = start;
        cell.lifespanDays = lifespanFor(type);

        // Count the starting location as a visit.
        cell.visits[start]++;

        for (int step = 0;
             step < maxStepsPerCell && cell.ageDays < cell.lifespanDays;
             ++step) {

            double edgeDistance = 0.0;
            int next = chooseNextVertex(cell.currentVertex, edgeDistance);

            // Safety check in case a vertex has no outgoing path.
            if (next == cell.currentVertex &&
                graph_.neighbors(cell.currentVertex).empty()) {
                break;
            }

            // Update the profile after one movement through an edge.
            cell.currentVertex = next;
            cell.stepsTaken++;
            cell.ageDays += daysPerStep(type);
            cell.distanceTravelledMeters += edgeDistance;
            cell.visits[next]++;

            if (next == cycleMarker) {
                cell.completedCycles++;
            }
        }

        profiles.push_back(std::move(cell));
    }

    return profiles;
}

// -------------------- Profiling --------------------
// Combines the results from all cells of one type and prints the final statistics.
void Simulation::printSummary(CellType type, const std::vector<CellProfile>& profiles) const {
    std::unordered_map<int, long long> totalVisits;

    long long organVisits = 0;
    long long totalSteps = 0;
    long long totalCycles = 0;
    double totalDistance = 0.0;

    // Add together movement, cycle, distance and visit data from every cell.
    for (const auto& cell : profiles) {
        totalSteps += cell.stepsTaken;
        totalCycles += cell.completedCycles;
        totalDistance += cell.distanceTravelledMeters;

        for (const auto& [vertex, count] : cell.visits) {
            totalVisits[vertex] += count;

            if (graph_.vertex(vertex).isOrgan) {
                organVisits += count;
            }
        }
    }

    std::cout << "\n=== " << cellTypeName(type) << " Profile ===\n";
    std::cout << "Cells simulated      : " << profiles.size() << '\n';
    std::cout << "Total movement steps : " << totalSteps << '\n';
    std::cout << "Total cycles         : " << totalCycles << '\n';
    std::cout << "Total distance       : " << std::fixed << std::setprecision(2)
              << totalDistance << " m\n";

    std::cout << "\nOrgan traversal:\n";

    for (int i = 0; i < graph_.vertexCount(); ++i) {
        if (!graph_.vertex(i).isOrgan) continue;

        long long visits = totalVisits[i];

        // Probability here means the percentage of all organ visits
        // that belong to this particular organ.
        double probability =
            organVisits == 0
                ? 0.0
                : (static_cast<double>(visits) / organVisits) * 100.0;

        std::cout << "  " << std::left << std::setw(20) << graph_.vertex(i).name
                  << std::right << std::setw(8) << visits
                  << " visits  " << std::setw(6) << std::setprecision(2)
                  << probability << "%\n";
    }
}

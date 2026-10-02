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

double Simulation::lifespanFor(CellType type) const {
    switch (type) {
        case CellType::RedBloodCell: return 120.0;
        case CellType::WhiteBloodCell: return 20.0;
        case CellType::Platelet: return 9.0;
    }
    return 1.0;
}

double Simulation::daysPerStep(CellType type) const {
    switch (type) {
        case CellType::RedBloodCell: return 2.0;
        case CellType::WhiteBloodCell: return 1.0;
        case CellType::Platelet: return 0.5;
    }
    return 1.0;
}

int Simulation::chooseNextVertex(int current, double& edgeDistance) {
    const auto& edges = graph_.neighbors(current);
    if (edges.empty()) {
        edgeDistance = 0.0;
        return current;
    }

    std::vector<double> weights;
    for (const auto& edge : edges) weights.push_back(std::max(0.001, edge.flowRateLpm));

    std::discrete_distribution<int> distribution(weights.begin(), weights.end());
    const auto& selected = edges[distribution(rng_)];
    edgeDistance = selected.distanceMeters;
    return selected.to;
}

std::vector<CellProfile> Simulation::run(CellType type, int cellCount, int maxStepsPerCell) {
    std::vector<CellProfile> profiles;
    profiles.reserve(cellCount);

    const int start = graph_.indexOf("Right Atrium");
    const int cycleMarker = graph_.indexOf("Left Ventricle");

    for (int id = 1; id <= cellCount; ++id) {
        CellProfile cell;
        cell.id = id;
        cell.type = type;
        cell.currentVertex = start;
        cell.lifespanDays = lifespanFor(type);
        cell.visits[start]++;

        for (int step = 0; step < maxStepsPerCell && cell.ageDays < cell.lifespanDays; ++step) {
            double edgeDistance = 0.0;
            int next = chooseNextVertex(cell.currentVertex, edgeDistance);
            if (next == cell.currentVertex && graph_.neighbors(cell.currentVertex).empty()) break;

            cell.currentVertex = next;
            cell.stepsTaken++;
            cell.ageDays += daysPerStep(type);
            cell.distanceTravelledMeters += edgeDistance;
            cell.visits[next]++;
            if (next == cycleMarker) cell.completedCycles++;
        }
        profiles.push_back(std::move(cell));
    }
    return profiles;
}

void Simulation::printSummary(CellType type, const std::vector<CellProfile>& profiles) const {
    std::unordered_map<int, long long> totalVisits;
    long long organVisits = 0;
    long long totalSteps = 0;
    long long totalCycles = 0;
    double totalDistance = 0.0;

    for (const auto& cell : profiles) {
        totalSteps += cell.stepsTaken;
        totalCycles += cell.completedCycles;
        totalDistance += cell.distanceTravelledMeters;
        for (const auto& [vertex, count] : cell.visits) {
            totalVisits[vertex] += count;
            if (graph_.vertex(vertex).isOrgan) organVisits += count;
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
        double probability = organVisits == 0 ? 0.0 :
            (static_cast<double>(visits) / organVisits) * 100.0;

        std::cout << "  " << std::left << std::setw(20) << graph_.vertex(i).name
                  << std::right << std::setw(8) << visits
                  << " visits  " << std::setw(6) << std::setprecision(2)
                  << probability << "%\n";
    }
}

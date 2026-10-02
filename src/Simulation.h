#pragma once

#include "CirculatoryGraph.h"

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

enum class CellType { RedBloodCell, WhiteBloodCell, Platelet };

struct CellProfile {
    int id{};
    CellType type{};
    int currentVertex{};
    int stepsTaken{};
    int completedCycles{};
    double ageDays{};
    double lifespanDays{};
    double distanceTravelledMeters{};
    std::unordered_map<int, int> visits;
};

class Simulation {
public:
    explicit Simulation(const CirculatoryGraph& graph);

    std::vector<CellProfile> run(CellType type, int cellCount, int maxStepsPerCell);
    void printSummary(CellType type, const std::vector<CellProfile>& profiles) const;

    static std::string cellTypeName(CellType type);

private:
    int chooseNextVertex(int current, double& edgeDistance);
    double lifespanFor(CellType type) const;
    double daysPerStep(CellType type) const;

    const CirculatoryGraph& graph_;
    std::mt19937 rng_;
};

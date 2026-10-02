#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Edge {
    int to{};
    double distanceMeters{};
    double flowRateLpm{};
};

struct Vertex {
    std::string name;
    bool isOrgan{};
};

class CirculatoryGraph {
public:
    int addVertex(const std::string& name, bool isOrgan = false);
    void addEdge(const std::string& from, const std::string& to,
                 double distanceMeters, double flowRateLpm);

    int indexOf(const std::string& name) const;
    const Vertex& vertex(int index) const;
    const std::vector<Edge>& neighbors(int index) const;
    int vertexCount() const;

    std::vector<int> bfs(int start) const;
    std::vector<int> dfs(int start) const;
    std::pair<double, std::vector<int>> dijkstra(int start, int destination) const;

    void printGraph() const;
    void exportDot(const std::string& filename) const;

private:
    void dfsVisit(int current, std::vector<bool>& visited,
                  std::vector<int>& order) const;

    std::vector<Vertex> vertices_;
    std::vector<std::vector<Edge>> adjacency_;
    std::unordered_map<std::string, int> nameToIndex_;
};

CirculatoryGraph buildDefaultCirculatoryGraph();

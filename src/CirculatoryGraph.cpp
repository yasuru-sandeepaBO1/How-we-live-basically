#include "CirculatoryGraph.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>

int CirculatoryGraph::addVertex(const std::string& name, bool isOrgan) {
    if (nameToIndex_.count(name)) return nameToIndex_.at(name);
    int index = static_cast<int>(vertices_.size());
    vertices_.push_back({name, isOrgan});
    adjacency_.push_back({});
    nameToIndex_[name] = index;
    return index;
}

void CirculatoryGraph::addEdge(const std::string& from, const std::string& to,
                               double distanceMeters, double flowRateLpm) {
    int fromIndex = indexOf(from);
    int toIndex = indexOf(to);
    adjacency_[fromIndex].push_back({toIndex, distanceMeters, flowRateLpm});
}

int CirculatoryGraph::indexOf(const std::string& name) const {
    auto it = nameToIndex_.find(name);
    if (it == nameToIndex_.end()) throw std::runtime_error("Unknown graph vertex: " + name);
    return it->second;
}

const Vertex& CirculatoryGraph::vertex(int index) const { return vertices_.at(index); }
const std::vector<Edge>& CirculatoryGraph::neighbors(int index) const { return adjacency_.at(index); }
int CirculatoryGraph::vertexCount() const { return static_cast<int>(vertices_.size()); }

std::vector<int> CirculatoryGraph::bfs(int start) const {
    std::vector<bool> visited(vertices_.size(), false);
    std::vector<int> order;
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        order.push_back(current);
        for (const auto& edge : adjacency_[current]) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                q.push(edge.to);
            }
        }
    }
    return order;
}

void CirculatoryGraph::dfsVisit(int current, std::vector<bool>& visited,
                                std::vector<int>& order) const {
    visited[current] = true;
    order.push_back(current);
    for (const auto& edge : adjacency_[current]) {
        if (!visited[edge.to]) dfsVisit(edge.to, visited, order);
    }
}

std::vector<int> CirculatoryGraph::dfs(int start) const {
    std::vector<bool> visited(vertices_.size(), false);
    std::vector<int> order;
    dfsVisit(start, visited, order);
    return order;
}

std::pair<double, std::vector<int>> CirculatoryGraph::dijkstra(int start, int destination) const {
    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> distance(vertices_.size(), INF);
    std::vector<int> previous(vertices_.size(), -1);
    using Item = std::pair<double, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    distance[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [currentDistance, current] = pq.top();
        pq.pop();
        if (currentDistance > distance[current]) continue;
        if (current == destination) break;

        for (const auto& edge : adjacency_[current]) {
            double candidate = currentDistance + edge.distanceMeters;
            if (candidate < distance[edge.to]) {
                distance[edge.to] = candidate;
                previous[edge.to] = current;
                pq.push({candidate, edge.to});
            }
        }
    }

    std::vector<int> path;
    if (distance[destination] == INF) return {INF, path};

    for (int at = destination; at != -1; at = previous[at]) path.push_back(at);
    std::reverse(path.begin(), path.end());
    return {distance[destination], path};
}

void CirculatoryGraph::printGraph() const {
    std::cout << "\n--- Circulatory Graph ---\n";
    for (int i = 0; i < vertexCount(); ++i) {
        std::cout << vertices_[i].name << " -> ";
        if (adjacency_[i].empty()) std::cout << "(none)";
        for (std::size_t j = 0; j < adjacency_[i].size(); ++j) {
            const auto& edge = adjacency_[i][j];
            std::cout << vertices_[edge.to].name << " ["
                      << std::fixed << std::setprecision(2)
                      << edge.distanceMeters << " m, "
                      << edge.flowRateLpm << " L/min]";
            if (j + 1 < adjacency_[i].size()) std::cout << ", ";
        }
        std::cout << '\n';
    }
}

void CirculatoryGraph::exportDot(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Could not write " + filename);

    out << "digraph Circulation {\n";
    out << "  rankdir=LR;\n";
    out << "  graph [bgcolor=\"white\", nodesep=0.7, ranksep=1.0];\n";
    out << "  node [shape=box, style=\"rounded,filled\", fillcolor=\"#f7f7f7\", fontname=\"Arial\"];\n";
    out << "  edge [fontname=\"Arial\", fontsize=10];\n\n";

    for (const auto& v : vertices_) out << "  \"" << v.name << "\";\n";
    out << '\n';

    for (int from = 0; from < vertexCount(); ++from) {
        for (const auto& edge : adjacency_[from]) {
            out << "  \"" << vertices_[from].name << "\" -> \""
                << vertices_[edge.to].name << "\" [label=\""
                << std::fixed << std::setprecision(2)
                << edge.distanceMeters << " m | " << edge.flowRateLpm
                << " L/min\"];\n";
        }
    }
    out << "}\n";
}

CirculatoryGraph buildDefaultCirculatoryGraph() {
    CirculatoryGraph graph;
    graph.addVertex("Right Atrium");
    graph.addVertex("Right Ventricle");
    graph.addVertex("Lungs", true);
    graph.addVertex("Left Atrium");
    graph.addVertex("Left Ventricle");
    graph.addVertex("Brain", true);
    graph.addVertex("Liver", true);
    graph.addVertex("Kidneys", true);
    graph.addVertex("Muscles", true);
    graph.addVertex("Digestive System", true);

    graph.addEdge("Right Atrium", "Right Ventricle", 0.08, 5.0);
    graph.addEdge("Right Ventricle", "Lungs", 0.20, 5.0);
    graph.addEdge("Lungs", "Left Atrium", 0.20, 5.0);
    graph.addEdge("Left Atrium", "Left Ventricle", 0.08, 5.0);
    graph.addEdge("Left Ventricle", "Brain", 0.45, 0.75);
    graph.addEdge("Left Ventricle", "Liver", 0.55, 1.25);
    graph.addEdge("Left Ventricle", "Kidneys", 0.60, 1.00);
    graph.addEdge("Left Ventricle", "Muscles", 0.80, 1.50);
    graph.addEdge("Left Ventricle", "Digestive System", 0.70, 0.50);
    graph.addEdge("Brain", "Right Atrium", 0.45, 0.75);
    graph.addEdge("Liver", "Right Atrium", 0.55, 1.25);
    graph.addEdge("Kidneys", "Right Atrium", 0.60, 1.00);
    graph.addEdge("Muscles", "Right Atrium", 0.80, 1.50);
    graph.addEdge("Digestive System", "Right Atrium", 0.70, 0.50);

    return graph;
}

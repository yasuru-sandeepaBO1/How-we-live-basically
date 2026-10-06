#include "CirculatoryGraph.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>

// Graph construction
int CirculatoryGraph::addVertex(const std::string& name, bool isOrgan) {
    if (nameToIndex_.count(name)) return nameToIndex_.at(name);
    int index = static_cast<int>(vertices_.size());
    vertices_.push_back({name, isOrgan});
    adjacency_.push_back({});
    nameToIndex_[name] = index;
    return index;
}

void CirculatoryGraph::addEdge(const std::string& from, const std::string& to,
                               double distanceMeters, double flowRateLpm,
                               RouteType routeType) {
    int fromIndex = indexOf(from);
    int toIndex = indexOf(to);
    adjacency_[fromIndex].push_back({toIndex, distanceMeters, flowRateLpm, routeType});
}

int CirculatoryGraph::indexOf(const std::string& name) const {
    auto it = nameToIndex_.find(name);
    if (it == nameToIndex_.end()) throw std::runtime_error("Unknown graph vertex: " + name);
    return it->second;
}

const Vertex& CirculatoryGraph::vertex(int index) const { return vertices_.at(index); }
const std::vector<Edge>& CirculatoryGraph::neighbors(int index) const { return adjacency_.at(index); }
int CirculatoryGraph::vertexCount() const { return static_cast<int>(vertices_.size()); }

// BFS implementation
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

// DFS implementation
void CirculatoryGraph::dfsVisit(int current, std::vector<bool>& visited,
                                std::vector<int>& order) const {
    visited[current] = true;
    order.push_back(current);

    for (const auto& edge : adjacency_[current]) {
        if (!visited[edge.to]) {
            dfsVisit(edge.to, visited, order);
        }
    }
}

std::vector<int> CirculatoryGraph::dfs(int start) const {
    std::vector<bool> visited(vertices_.size(), false);
    std::vector<int> order;

    dfsVisit(start, visited, order);
    return order;
}

// Dijkstra implementation
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

    for (int at = destination; at != -1; at = previous[at]) {
        path.push_back(at);
    }

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

// Graphviz DOT export
void CirculatoryGraph::exportDot(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Could not write " + filename);

    out << "digraph Circulation {\n";
    out << "  rankdir=LR;\n";
    out << "  graph [bgcolor=\"white\", nodesep=0.7, ranksep=1.0];\n";
    out << "  node [shape=box, style=\"rounded,filled\", fillcolor=\"#f7f7f7\", fontname=\"Arial\"];\n";
    out << "  edge [fontname=\"Arial\", fontsize=10];\n\n";

    for (const auto& v : vertices_) {
        out << "  \"" << v.name << "\";\n";
    }

    out << '\n';

    for (int from = 0; from < vertexCount(); ++from) {
        for (const auto& edge : adjacency_[from]) {
            const char* color =
                edge.routeType == RouteType::OxygenRich ? "#e45b78" :
                edge.routeType == RouteType::Portal ? "#9b6bd6" :
                "#4c8bd6";

            out << "  \"" << vertices_[from].name << "\" -> \""
                << vertices_[edge.to].name << "\" [color=\"" << color << "\", label=\""
                << std::fixed << std::setprecision(2)
                << edge.distanceMeters << " m | "
                << edge.flowRateLpm << " L/min\"];\n";
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
    graph.addVertex("Aorta");
    graph.addVertex("Venae Cavae");
    graph.addVertex("Brain", true);
    graph.addVertex("Heart Muscle", true);
    graph.addVertex("Kidneys", true);
    graph.addVertex("Liver", true);
    graph.addVertex("Digestive Tract", true);
    graph.addVertex("Spleen", true);
    graph.addVertex("Other Tissues", true);

    graph.addEdge("Right Atrium", "Right Ventricle", 0.08, 5.0, RouteType::OxygenPoor);
    graph.addEdge("Right Ventricle", "Lungs", 0.20, 5.0, RouteType::OxygenPoor);
    graph.addEdge("Lungs", "Left Atrium", 0.20, 5.0, RouteType::OxygenRich);
    graph.addEdge("Left Atrium", "Left Ventricle", 0.08, 5.0, RouteType::OxygenRich);
    graph.addEdge("Left Ventricle", "Aorta", 0.12, 5.0, RouteType::OxygenRich);

    graph.addEdge("Aorta", "Brain", 0.45, 0.75, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Heart Muscle", 0.18, 0.25, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Kidneys", 0.60, 1.00, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Liver", 0.55, 0.35, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Digestive Tract", 0.70, 1.10, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Spleen", 0.68, 0.30, RouteType::OxygenRich);
    graph.addEdge("Aorta", "Other Tissues", 0.80, 1.25, RouteType::OxygenRich);

    graph.addEdge("Brain", "Venae Cavae", 0.45, 0.75, RouteType::OxygenPoor);
    graph.addEdge("Heart Muscle", "Venae Cavae", 0.18, 0.25, RouteType::OxygenPoor);
    graph.addEdge("Kidneys", "Venae Cavae", 0.60, 1.00, RouteType::OxygenPoor);
    graph.addEdge("Liver", "Venae Cavae", 0.55, 1.45, RouteType::OxygenPoor);
    graph.addEdge("Other Tissues", "Venae Cavae", 0.80, 1.25, RouteType::OxygenPoor);
    graph.addEdge("Venae Cavae", "Right Atrium", 0.15, 5.0, RouteType::OxygenPoor);

    graph.addEdge("Digestive Tract", "Liver", 0.28, 1.10, RouteType::Portal);
    graph.addEdge("Spleen", "Liver", 0.24, 0.30, RouteType::Portal);

    return graph;
}

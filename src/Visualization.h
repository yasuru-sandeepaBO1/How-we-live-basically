#pragma once

#include "CirculatoryGraph.h"
#include "Simulation.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <unordered_map>
#include <vector>

class Visualization {
public:
    explicit Visualization(const CirculatoryGraph& graph);
    int run();

private:
    struct Particle {
        CellType type{CellType::RedBloodCell};
        int from{};
        int to{};
        float progress{};
        float speed{};
    };

    struct Button {
        sf::FloatRect bounds;
        std::string label;
    };

    bool loadFont();
    void setupPositions();
    void createParticles();
    void updateParticles(float dt);
    void chooseNextEdge(Particle& particle, int currentVertex);

    void drawDashboard(sf::RenderWindow& window);
    void drawGraph(sf::RenderWindow& window);
    void drawEdge(sf::RenderWindow& window, sf::Vector2f from, sf::Vector2f to,
                  const sf::Color& color, float thickness = 2.0f);
    void drawNode(sf::RenderWindow& window, int vertexIndex, const sf::Vector2f& position);
    void drawParticles(sf::RenderWindow& window);
    void drawSidePanel(sf::RenderWindow& window);
    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y,
                  unsigned size, sf::Color color, bool bold = false);
    void drawLegendItem(sf::RenderWindow& window, float x, const std::string& label,
                        const sf::Color& color);

    sf::Color routeColor(RouteType type) const;
    sf::Color cellColor(CellType type) const;
    std::string topVisitedOrgan() const;

    const CirculatoryGraph& graph_;
    std::unordered_map<int, sf::Vector2f> positions_;
    std::vector<Particle> particles_;
    std::unordered_map<int, long long> organVisits_;
    std::mt19937 rng_;
    sf::Font font_;
    bool fontLoaded_{false};
    bool paused_{false};
    float speedMultiplier_{1.0f};
    long long completedMoves_{0};
    Button pauseButton_{{950.f, 180.f, 135.f, 50.f}, "Pause"};
    Button speedButton_{{1100.f, 180.f, 135.f, 50.f}, "Speed 1x"};
};

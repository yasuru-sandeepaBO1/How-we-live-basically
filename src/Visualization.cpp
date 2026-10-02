#include "Visualization.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace {
constexpr float PI = 3.14159265358979323846f;
const sf::Color BG(10, 22, 40);
const sf::Color PANEL(15, 32, 55);
const sf::Color CARD(22, 44, 72);
const sf::Color BORDER(49, 75, 108);
const sf::Color TEXT(225, 235, 248);
const sf::Color MUTED(153, 174, 201);
const sf::Color ACCENT(79, 216, 196);
const sf::Color OXYGEN_RICH(229, 91, 120);
const sf::Color OXYGEN_POOR(76, 139, 214);
const sf::Color PORTAL(158, 111, 219);
const sf::Color RBC(255, 101, 126);
const sf::Color WBC(241, 245, 249);
const sf::Color PLATELET(246, 190, 72);
}

Visualization::Visualization(const CirculatoryGraph& graph)
    : graph_(graph), rng_(std::random_device{}()) {
    fontLoaded_ = loadFont();
    setupPositions();
    createParticles();
}

bool Visualization::loadFont() {
    const std::vector<std::string> candidates = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttf",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path) && font_.loadFromFile(path)) return true;
    }
    return false;
}

void Visualization::setupPositions() {
    auto set = [&](const std::string& name, float x, float y) {
        positions_[graph_.indexOf(name)] = {x, y};
    };

    set("Lungs", 450, 95);
    set("Right Atrium", 285, 250);
    set("Right Ventricle", 285, 355);
    set("Left Atrium", 615, 250);
    set("Left Ventricle", 615, 355);
    set("Venae Cavae", 165, 500);
    set("Aorta", 735, 500);
    set("Brain", 95, 655);
    set("Heart Muscle", 320, 655);
    set("Kidneys", 215, 770);
    set("Liver", 590, 655);
    set("Digestive Tract", 700, 770);
    set("Spleen", 825, 655);
    set("Other Tissues", 470, 790);
}

void Visualization::createParticles() {
    particles_.clear();
    const int start = graph_.indexOf("Right Atrium");

    auto add = [&](CellType type, int count, float baseSpeed) {
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.type = type;
            p.from = start;
            p.speed = baseSpeed * (0.85f + 0.30f * (static_cast<float>(i % 7) / 6.f));
            chooseNextEdge(p, start);
            p.progress = static_cast<float>(i) / static_cast<float>(std::max(1, count));
            particles_.push_back(p);
        }
    };

    add(CellType::RedBloodCell, 34, 0.34f);
    add(CellType::WhiteBloodCell, 8, 0.27f);
    add(CellType::Platelet, 14, 0.30f);
}

void Visualization::chooseNextEdge(Particle& particle, int currentVertex) {
    const auto& edges = graph_.neighbors(currentVertex);
    if (edges.empty()) {
        particle.from = currentVertex;
        particle.to = currentVertex;
        particle.progress = 0.f;
        return;
    }

    std::vector<double> weights;
    for (const auto& edge : edges) weights.push_back(std::max(0.001, edge.flowRateLpm));
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    const auto& selected = edges[dist(rng_)];

    particle.from = currentVertex;
    particle.to = selected.to;
    particle.progress = 0.f;
}

void Visualization::updateParticles(float dt) {
    if (paused_) return;

    for (auto& particle : particles_) {
        particle.progress += dt * particle.speed * speedMultiplier_;
        if (particle.progress >= 1.f) {
            int arrived = particle.to;
            completedMoves_++;
            if (graph_.vertex(arrived).isOrgan) organVisits_[arrived]++;
            chooseNextEdge(particle, arrived);
        }
    }
}

sf::Color Visualization::routeColor(RouteType type) const {
    if (type == RouteType::OxygenRich) return OXYGEN_RICH;
    if (type == RouteType::Portal) return PORTAL;
    return OXYGEN_POOR;
}

sf::Color Visualization::cellColor(CellType type) const {
    if (type == CellType::WhiteBloodCell) return WBC;
    if (type == CellType::Platelet) return PLATELET;
    return RBC;
}

void Visualization::drawText(sf::RenderWindow& window, const std::string& text, float x, float y,
                             unsigned size, sf::Color color, bool bold) {
    if (!fontLoaded_) return;
    sf::Text label;
    label.setFont(font_);
    label.setString(text);
    label.setCharacterSize(size);
    label.setFillColor(color);
    if (bold) label.setStyle(sf::Text::Bold);
    label.setPosition(x, y);
    window.draw(label);
}

void Visualization::drawEdge(sf::RenderWindow& window, sf::Vector2f from, sf::Vector2f to,
                             const sf::Color& color, float thickness) {
    sf::Vector2f diff = to - from;
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (length < 1.f) return;

    float angle = std::atan2(diff.y, diff.x) * 180.f / PI;
    sf::RectangleShape line({length, thickness});
    line.setPosition(from);
    line.setRotation(angle);
    line.setFillColor(sf::Color(color.r, color.g, color.b, 210));
    window.draw(line);

    sf::Vector2f arrowPos = from + diff * 0.62f;
    sf::CircleShape arrow(6.f, 3);
    arrow.setOrigin(6.f, 6.f);
    arrow.setPosition(arrowPos);
    arrow.setRotation(angle + 90.f);
    arrow.setFillColor(color);
    window.draw(arrow);
}

void Visualization::drawNode(sf::RenderWindow& window, int vertexIndex, const sf::Vector2f& position) {
    const std::string& name = graph_.vertex(vertexIndex).name;
    sf::Vector2f size(126.f, 48.f);
    if (name == "Venae Cavae" || name == "Digestive Tract" ||
        name == "Other Tissues" || name == "Heart Muscle") {
        size.x = 145.f;
    }

    sf::RectangleShape box(size);
    box.setOrigin(size.x / 2.f, size.y / 2.f);
    box.setPosition(position);
    box.setFillColor(CARD);
    box.setOutlineColor(name == "Lungs" ? ACCENT : BORDER);
    box.setOutlineThickness(name == "Lungs" ? 2.2f : 1.5f);
    window.draw(box);

    if (!fontLoaded_) return;
    sf::Text label;
    label.setFont(font_);
    label.setString(name);
    label.setCharacterSize(name.size() > 13 ? 13 : 14);
    label.setStyle(sf::Text::Bold);
    label.setFillColor(TEXT);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    label.setPosition(position);
    window.draw(label);
}

void Visualization::drawLegendItem(sf::RenderWindow& window, float x, const std::string& label,
                                   const sf::Color& color) {
    sf::RectangleShape dash({35.f, 5.f});
    dash.setPosition(x, 67.f);
    dash.setFillColor(color);
    window.draw(dash);
    drawText(window, label, x + 48.f, 55.f, 17, MUTED);
}

void Visualization::drawGraph(sf::RenderWindow& window) {
    drawText(window, "LIVE CIRCULATION GRAPH", 35, 20, 24, TEXT, true);
    drawLegendItem(window, 35, "Oxygen-rich route", OXYGEN_RICH);
    drawLegendItem(window, 255, "Oxygen-poor route", OXYGEN_POOR);
    drawLegendItem(window, 485, "Portal route", PORTAL);

    drawText(window, "PULMONARY CIRCULATION", 365, 135, 12, MUTED);
    drawText(window, "RIGHT HEART", 225, 195, 12, MUTED);
    drawText(window, "LEFT HEART", 565, 195, 12, MUTED);
    drawText(window, "SYSTEMIC CIRCULATION", 375, 560, 12, MUTED);

    for (int from = 0; from < graph_.vertexCount(); ++from) {
        auto fromIt = positions_.find(from);
        if (fromIt == positions_.end()) continue;
        for (const auto& edge : graph_.neighbors(from)) {
            auto toIt = positions_.find(edge.to);
            if (toIt == positions_.end()) continue;
            drawEdge(window, fromIt->second, toIt->second, routeColor(edge.routeType));
        }
    }

    for (const auto& [index, position] : positions_) drawNode(window, index, position);
    drawParticles(window);
}

void Visualization::drawParticles(sf::RenderWindow& window) {
    for (const auto& particle : particles_) {
        auto a = positions_.find(particle.from);
        auto b = positions_.find(particle.to);
        if (a == positions_.end() || b == positions_.end()) continue;

        sf::Vector2f pos = a->second + (b->second - a->second) * particle.progress;
        float radius = particle.type == CellType::WhiteBloodCell ? 5.f : 4.2f;
        sf::CircleShape dot(radius);
        dot.setOrigin(radius, radius);
        dot.setPosition(pos);
        dot.setFillColor(cellColor(particle.type));
        dot.setOutlineColor(BG);
        dot.setOutlineThickness(1.f);
        window.draw(dot);
    }
}

std::string Visualization::topVisitedOrgan() const {
    if (organVisits_.empty()) return "No visits yet";
    auto best = std::max_element(
        organVisits_.begin(), organVisits_.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return graph_.vertex(best->first).name;
}

void Visualization::drawSidePanel(sf::RenderWindow& window) {
    sf::RectangleShape panel({330.f, 860.f});
    panel.setPosition(920.f, 20.f);
    panel.setFillColor(PANEL);
    panel.setOutlineColor(BORDER);
    panel.setOutlineThickness(1.5f);
    window.draw(panel);

    sf::RectangleShape status({135.f, 42.f});
    status.setPosition(950.f, 55.f);
    status.setFillColor(paused_ ? sf::Color(75, 57, 65) : sf::Color(21, 63, 61));
    status.setOutlineColor(paused_ ? OXYGEN_RICH : ACCENT);
    status.setOutlineThickness(1.f);
    window.draw(status);
    drawText(window, paused_ ? "PAUSED" : "RUNNING", 973, 65, 18,
             paused_ ? OXYGEN_RICH : ACCENT, true);

    drawText(window, "Simulation controls", 950, 125, 24, TEXT, true);

    sf::RectangleShape pause({pauseButton_.bounds.width, pauseButton_.bounds.height});
    pause.setPosition(pauseButton_.bounds.left, pauseButton_.bounds.top);
    pause.setFillColor(ACCENT);
    window.draw(pause);
    drawText(window, paused_ ? "Resume" : "Pause", 983, 193, 19, BG, true);

    sf::RectangleShape speed({speedButton_.bounds.width, speedButton_.bounds.height});
    speed.setPosition(speedButton_.bounds.left, speedButton_.bounds.top);
    speed.setFillColor(CARD);
    speed.setOutlineColor(BORDER);
    speed.setOutlineThickness(1.5f);
    window.draw(speed);

    std::ostringstream speedLabel;
    speedLabel << "Speed ";
    if (speedMultiplier_ == 0.5f) speedLabel << "0.5";
    else speedLabel << static_cast<int>(speedMultiplier_);
    speedLabel << "x";
    drawText(window, speedLabel.str(), 1114, 193, 18, TEXT, true);

    drawText(window, "Cardiac output", 950, 275, 17, MUTED);
    drawText(window, "5.0 L/min", 1110, 270, 22, TEXT, true);

    drawText(window, "Blood cells on screen", 950, 335, 17, MUTED);
    drawText(window, "RBC", 950, 375, 15, RBC, true);
    drawText(window, "34", 1190, 375, 15, TEXT, true);
    drawText(window, "WBC", 950, 407, 15, WBC, true);
    drawText(window, "8", 1190, 407, 15, TEXT, true);
    drawText(window, "Platelets", 950, 439, 15, PLATELET, true);
    drawText(window, "14", 1190, 439, 15, TEXT, true);

    sf::RectangleShape separator({270.f, 1.f});
    separator.setPosition(950.f, 490.f);
    separator.setFillColor(BORDER);
    window.draw(separator);

    drawText(window, "Live profiling", 950, 520, 24, TEXT, true);
    drawText(window, "Completed edge moves", 950, 570, 16, MUTED);
    drawText(window, std::to_string(completedMoves_), 950, 596, 25, TEXT, true);
    drawText(window, "Most visited organ", 950, 655, 16, MUTED);
    drawText(window, topVisitedOrgan(), 950, 681, 20, TEXT, true);

    long long totalOrganVisits = 0;
    for (const auto& [_, count] : organVisits_) totalOrganVisits += count;
    drawText(window, "Total organ visits", 950, 735, 16, MUTED);
    drawText(window, std::to_string(totalOrganVisits), 950, 761, 25, TEXT, true);

    drawText(window, "RBC", 950, 820, 13, RBC, true);
    drawText(window, "WBC", 1010, 820, 13, WBC, true);
    drawText(window, "PLT", 1075, 820, 13, PLATELET, true);
}

void Visualization::drawDashboard(sf::RenderWindow& window) {
    window.clear(BG);
    drawGraph(window);
    drawSidePanel(window);
}

int Visualization::run() {
    sf::RenderWindow window(
        sf::VideoMode(1280, 900),
        "Blood Circulation Graph Simulator",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space) {
                paused_ = !paused_;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouse(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                if (pauseButton_.bounds.contains(mouse)) {
                    paused_ = !paused_;
                } else if (speedButton_.bounds.contains(mouse)) {
                    if (speedMultiplier_ < 1.f) speedMultiplier_ = 1.f;
                    else if (speedMultiplier_ < 2.f) speedMultiplier_ = 2.f;
                    else if (speedMultiplier_ < 5.f) speedMultiplier_ = 5.f;
                    else if (speedMultiplier_ < 10.f) speedMultiplier_ = 10.f;
                    else speedMultiplier_ = 0.5f;
                }
            }
        }

        float dt = std::min(clock.restart().asSeconds(), 0.05f);
        updateParticles(dt);
        drawDashboard(window);
        window.display();
    }
    return 0;
}

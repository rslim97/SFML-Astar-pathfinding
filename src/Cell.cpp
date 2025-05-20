#include <SFML/Graphics.hpp>
#include <Coordinate.hpp>
#include <Cell.hpp>
#include <limits>


Cell::Cell(int cellSize) {
    m_marked = false;
    m_distance = std::numeric_limits<float>::infinity();
    m_weight = 1.0;
    m_type = Type::Unexplored;
    m_last_coord = Coordinate(-1, -1);
    m_rect = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
    m_rect.setFillColor(sf::Color::White);
}
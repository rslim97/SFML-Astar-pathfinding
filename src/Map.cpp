#include <SFML/Graphics.hpp>
#include <vector>
#include <Coordinate.hpp>
#include <Cell.hpp>
#include <Map.hpp>
#include <iostream>
using namespace std;

Map::Map(const Coordinate& start, const Coordinate& end,
    int cellSize, int mapWidth, int mapHeight, int marginWidth, int marginHeight)
    : m_start(start), m_end(end), m_cellSize(cellSize), m_mapWidth(mapWidth), m_mapHeight(mapHeight), 
    m_marginWidth(marginWidth), m_marginHeight(marginHeight) {
    cells.reserve(mapWidth * mapHeight);
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            cells.push_back(make_unique<Cell>(cellSize));
        }
    }
    initMap();
}

Map::~Map() {

}

void Map::putWall(const Coordinate& coord) {
    getCell(coord).m_type = Cell::Type::Obstacle;
}

void Map::removeWall(const Coordinate& coord) {
    getCell(coord).m_type = Cell::Type::Unexplored;
    getCell(coord).m_marked = false;
    getCell(coord).m_distance = std::numeric_limits<float>::infinity();
}

Cell& Map::getCell(const Coordinate& coord) {
    return *cells.at(coord.y * m_mapWidth + coord.x);
}

void Map::draw(sf::RenderWindow& window, const vector<Coordinate>& path) {
    for (int i = 0; i < m_mapWidth; ++i) {
        for (int j = 0; j < m_mapHeight; ++j) {
            if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::Frontier) {
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color(200,200,200));
            }
            else if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::Explored) {
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color(100,100,100));
            }
            else if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::Obstacle) {
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color(0,0,0));
            }
            else if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::Start) {
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::Red);
            }
            else if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::End) {
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::Green);
            }
        }
    }
    // render path
    for (auto& node: path) {
        cells.at(node.y * m_mapWidth + node.x)->m_rect.setFillColor(sf::Color::Blue);
    }
    for (int i = 0; i < m_mapWidth; ++i) {
        for (int j = 0; j < m_mapHeight; ++j) {
            window.draw(sf::RectangleShape(cells.at(j * m_mapWidth + i)->m_rect));
        }
    }
}

void Map::initMap() {
    for (int i = 0; i < m_mapWidth; ++i) {
        for (int j = 0; j < m_mapHeight; ++j) {
            if (i == m_start.x && j == m_start.y) {
                cells.at(j * m_mapWidth + i)->m_type = Cell::Type::Start;
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::Green);
                cells.at(j * m_mapWidth + i)->m_distance = 0.0;
            }
            else if (i == m_end.x && j == m_end.y) {
                cells.at(j * m_mapWidth + i)->m_type = Cell::Type::End;
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::Red);
            }
            else {
                cells.at(j * m_mapWidth + i)->m_type = Cell::Type::Unexplored;
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::White);
            }
            cells.at(j * m_mapWidth + i)->m_rect.setPosition(sf::Vector2f(i * m_cellSize + m_marginWidth, j * m_cellSize + m_marginHeight));
            cells.at(j * m_mapWidth + i)->m_rect.setOutlineThickness(1.0f);
            cells.at(j * m_mapWidth + i)->m_rect.setOutlineColor(sf::Color::Black);
        }
    }
}

int Map::getMapWidth() {return m_mapWidth;}
int Map::getMapHeight() {return m_mapHeight;}

void Map::clearMap() {
    for (int i = 0; i < m_mapWidth; ++i) {
        for (int j = 0; j < m_mapHeight; ++j) {
            // reset start
            if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::Start) {
                cells.at(j * m_mapWidth + i)->m_distance = 0.0;
            }
            else if (cells.at(j * m_mapWidth + i)->m_type == Cell::Type::End) {
                cells.at(j * m_mapWidth + i)->m_distance = std::numeric_limits<float>::infinity();
            }
            // remove obstacles
            else if (cells.at(j * m_mapWidth + i)->m_type != Cell::Type::Start && 
                    cells.at(j * m_mapWidth + i)->m_type != Cell::Type::End) {
                cells.at(j * m_mapWidth + i)->m_type = Cell::Type::Unexplored;
                cells.at(j * m_mapWidth + i)->m_distance = std::numeric_limits<float>::infinity();
                cells.at(j * m_mapWidth + i)->m_rect.setFillColor(sf::Color::White);
            }
            cells.at(j * m_mapWidth + i)->m_marked = false;
            cells.at(j * m_mapWidth + i)->m_last_coord = Coordinate(-1, -1);

        }
    }
}
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <queue>
#include <vector>
#include <Coordinate.hpp>
#include <Cell.hpp>
#include <Map.hpp>
#include <Path.hpp>
#include <iostream>
using namespace std;

Path::Path(Map& my_map, const Coordinate& start, const Coordinate& end, const pathFindingMethod method): 
        method(method), m_pq(), m_match(false), m_search_active(false) {
            m_moves = {{-1,0},{0,-1},{1,0},{0,1}};
            m_mapWidth = my_map.getMapWidth();
            m_mapHeight = my_map.getMapHeight();
        }

float Path::getDistance(const std::pair<int, int>& move) {
    return sqrt(pow(move.first, 2) + pow(move.second, 2));
}

float Path::Heuristic(const Coordinate& a, const Coordinate& b, float weight) {
    // use weight > 1.0 for weighted Astar
    return static_cast<float>(weight * (abs(a.x - b.x) + abs(a.y - b.y)));  // Manhattan distance.
}

void Path::clearSearch(Map& my_map) {
    while (!m_pq.empty()) {
        m_pq.pop();
    }
    my_map.clearMap();
    m_path.clear();
}

void Path::initSearch(Map& my_map, const Coordinate& start, const Coordinate& end) {
    if (!m_search_active) {
        clearSearch(my_map);
        m_search_active = true;
        m_pq.push(start);
    }
}

bool Path::floodFill(Map& my_map, const Coordinate& start, const Coordinate& end, mt19937& gen) {
    // reset m_match
    m_match = false;
    if (!m_match && m_search_active) {
        switch (method) {
            case pathFindingMethod::Djikstra:
                m_match = DjikstraFlood(my_map, start, end, gen);
                break;
        }
        switch (method) {
            case pathFindingMethod::Astar:
                m_match = AstarFlood(my_map, start, end, gen);
                break;
        }
    }
    
    if (m_match == true) {
        savePath(my_map, start, end);
        m_search_active = false;
    }

    return m_match;
}

bool Path::DjikstraFlood(Map& my_map, const Coordinate& start, const Coordinate& end, mt19937& gen) {
    if (!m_pq.empty()) {
        Coordinate curr_c = m_pq.top();
        m_pq.pop();
        if (curr_c == end) return true;  // found end goal
        if (my_map.getCell(curr_c).m_marked == true) return false;
        my_map.getCell(curr_c).m_marked = true;
        // exclude start and end cell type to not be reassigned!
        if (my_map.getCell(curr_c).m_type != Cell::Type::Start && 
            my_map.getCell(curr_c).m_type != Cell::Type::End)
            my_map.getCell(curr_c).m_type = Cell::Type::Explored;
        // visit neighbor
        shuffle(m_moves.begin(), m_moves.end(), gen);
        for (const auto& move: m_moves) {
            float next_x = curr_c.x + move.first;
            float next_y = curr_c.y + move.second;
            Coordinate next_c = Coordinate(next_x, next_y);
            if (next_c.x >= 0 && next_c.x < m_mapWidth &&
                next_c.y >= 0 && next_c.y < m_mapHeight &&
                my_map.getCell(next_c).m_type != Cell::Type::Obstacle) {
                    float alt = my_map.getCell(curr_c).m_distance + my_map.getCell(next_c).m_weight * getDistance(move);
                    if (alt < my_map.getCell(next_c).m_distance) {
                        my_map.getCell(next_c).m_distance = alt;
                        my_map.getCell(next_c).m_last_coord = curr_c;
                        if (my_map.getCell(next_c).m_type != Cell::Type::Start && 
                            my_map.getCell(next_c).m_type != Cell::Type::End)
                            my_map.getCell(next_c).m_type = Cell::Type::Frontier;   
                        next_c.heuristic = alt;
                        m_pq.push(next_c);
                    }
                }
        }
        return false;
    }
    return true;  // exhausted all cells
}

bool Path::AstarFlood(Map& my_map, const Coordinate& start, const Coordinate& end, mt19937& gen) {
    if (!m_pq.empty()) {
        Coordinate curr_c = m_pq.top();
        m_pq.pop();
        if (curr_c == end) return true;  // found end goal
        if (my_map.getCell(curr_c).m_marked == true) return false;
        my_map.getCell(curr_c).m_marked = true;
        // exclude start and end cell type to not be reassigned!
        if (my_map.getCell(curr_c).m_type != Cell::Type::Start && 
            my_map.getCell(curr_c).m_type != Cell::Type::End)
            my_map.getCell(curr_c).m_type = Cell::Type::Explored;
        // visit neighbor
        shuffle(m_moves.begin(), m_moves.end(), gen);
        for (const auto& move: m_moves) {
            float next_x = curr_c.x + move.first;
            float next_y = curr_c.y + move.second;
            Coordinate next_c = Coordinate(next_x, next_y);
            if (next_c.x >= 0 && next_c.x < m_mapWidth &&
                next_c.y >= 0 && next_c.y < m_mapHeight &&
                my_map.getCell(next_c).m_type != Cell::Type::Obstacle) {
                    float alt = my_map.getCell(curr_c).m_distance + my_map.getCell(next_c).m_weight * getDistance(move);
                    if (alt < my_map.getCell(next_c).m_distance) {
                        my_map.getCell(next_c).m_distance = alt;
                        my_map.getCell(next_c).m_last_coord = curr_c;
                        // exclude start and end cell type to not be reassigned!
                        if (my_map.getCell(next_c).m_type != Cell::Type::Start && 
                            my_map.getCell(next_c).m_type != Cell::Type::End)
                            my_map.getCell(next_c).m_type = Cell::Type::Frontier;   
                        next_c.heuristic = alt + Heuristic(end, next_c, 1.0);
                        m_pq.push(next_c);
                    }
                }
        }
        return false;
    }
    return true;  // exhausted all cells
}

void Path::savePath(Map& my_map, const Coordinate& start, const Coordinate& end) {
    m_path.clear();
    Coordinate curr_c = end;
    while (my_map.getCell(curr_c).m_last_coord != Coordinate(-1, -1)) {
        if (curr_c != start && curr_c != end) {
            m_path.push_back(curr_c);
        }
        curr_c = my_map.getCell(curr_c).m_last_coord;
    }
}

vector<Coordinate> Path::getPath() {return m_path;}
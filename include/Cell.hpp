#ifndef CELL_H
#define CELL_H


class Cell {
public:
    bool m_marked;
    float m_distance;
    float m_weight;
    enum class Type {Obstacle, Unexplored, Explored, Frontier, Start, End};
    Type m_type;
    Coordinate m_last_coord;
    sf::RectangleShape m_rect;
    Cell(int cellSize);
};


#endif
#ifndef MAP_H
#define MAP_H


class Map {
public:
    Map(const Coordinate& start, const Coordinate& end, 
        int cellSize, int mapWidth, int mapHeight,
        int marginWidth, int marginHeight);
    ~Map();
    void putWall(const Coordinate& coord);
    void removeWall(const Coordinate& coord);
    Cell& getCell(const Coordinate& coord);
    int getMapWidth();
    int getMapHeight();
    void draw(sf::RenderWindow& window, const std::vector<Coordinate>& path);
    void clearMap();
private:
    std::vector<std::unique_ptr<Cell>> cells;
    Coordinate m_start;
    Coordinate m_end;
    void initMap();
    int m_cellSize, m_mapWidth, m_mapHeight, m_marginWidth, m_marginHeight;
};

#endif

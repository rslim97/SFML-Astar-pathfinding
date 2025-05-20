#ifndef PATH_H
#define PATH_H


class Path {
public:
    enum class pathFindingMethod {Djikstra, Astar};
    pathFindingMethod method;
    Path(Map& my_map, const Coordinate& start, const Coordinate& end, const pathFindingMethod method);
    void initSearch(Map& my_map, const Coordinate& start, const Coordinate& end);
    bool floodFill(Map& my_map, const Coordinate& start, const Coordinate& end, std::mt19937& gen);
    std::vector<Coordinate> getPath();
private:
    struct compareCellCoordinates {
        bool operator()(const Coordinate& a, const Coordinate& b) const {
            return a.heuristic > b.heuristic;
        }
    };
    float Heuristic(const Coordinate& left, const Coordinate& right, float weight);
    std::priority_queue<Coordinate, std::vector<Coordinate>, compareCellCoordinates> m_pq;
    // Coordinate m_start, m_end;
    std::vector<std::pair<int, int>> m_moves;
    int m_mapWidth;
    int m_mapHeight;
    // bool m_early_exit;
    bool m_match;
    bool m_search_active;
    std::vector<Coordinate> m_path;
    void clearSearch(Map& my_map);
    float getDistance(const std::pair<int, int>& move);
    void savePath(Map& my_map, const Coordinate& start, const Coordinate& end);
    bool DjikstraFlood(Map& my_map, const Coordinate& start, const Coordinate& end, std::mt19937& gen);
    bool AstarFlood(Map& my_map, const Coordinate& start, const Coordinate& end, std::mt19937& gen);
};

#endif
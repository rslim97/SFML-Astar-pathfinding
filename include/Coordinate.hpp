class Coordinate {
public:
    int x;
    int y;
    float heuristic;

    Coordinate(){}
    Coordinate(int x, int y):x(x), y(y), heuristic(0.0) {}

    Coordinate(const Coordinate& other) {
        x = other.x;
        y = other.y;
        heuristic = other.heuristic;
    }

    Coordinate& operator=(const Coordinate& other) {
        if (this == &other) {
            return *this;  // handle self-assignment
        }
        x = other.x;
        y = other.y;
        heuristic = other.heuristic;
        return *this;
    }

    bool operator==(const Coordinate& other) {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Coordinate& other) {
        return (x != other.x || y != other.y);
    }

};
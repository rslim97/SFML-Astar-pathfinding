#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <queue>
#include <random>
#include <cmath>

#include <Coordinate.hpp>
#include <Cell.hpp>
#include <Map.hpp>
#include <Path.hpp>
#include <numeric>

using namespace std;

long double s_elapsedTime = 0.0f;
long double s_totalTime = 0.0f;
float s_deltaTime = 0.005f;
const float s_timeStep = s_deltaTime;
int ctr;

inline const char* toString(Path::pathFindingMethod method) {
    switch (method)
    {
    case Path::pathFindingMethod::Astar: return "A*";
    case Path::pathFindingMethod::Djikstra: return "Djikstra";
    default:
        return "Unknown Path Finding method";
    }
}

int main(int argc, char* argv[]) {
    sf::Font open_sans;
    if (!open_sans.loadFromFile("OpenSans-Regular.ttf")) {
        cout<<"Could not load font."<<endl;
        return 0;
    }
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Render Window", sf::Style::Close, settings);

    int mapWidth = 50;
    int mapHeight = 50;
    int cellSize = 10;
    int marginWidth = (WINDOW_WIDTH - cellSize * mapWidth) / 2;
    int marginHeight = (WINDOW_HEIGHT - cellSize * mapHeight) / 2;

    // set path finding method
    Path::pathFindingMethod method = Path::pathFindingMethod::Astar;
    string s_title = toString(method);
    string s_command = "Press 'S' to start search. \nPress 'Esc' to exit.";
    sf::Text title_text;
    sf::Text command_text;
    sf::Text total_time_text;

    title_text.setFont(open_sans);
    title_text.setString(s_title);
    int titlePosX = WINDOW_WIDTH / 2 - title_text.getLocalBounds().width / 2;
    int titlePosY = 3 * marginHeight / 4;
    title_text.setPosition(titlePosX, titlePosY);
    title_text.setFillColor(sf::Color::White);

    total_time_text.setFont(open_sans);
    total_time_text.setString(s_command);
    int timePosX = WINDOW_WIDTH / 2 - total_time_text.getLocalBounds().width / 2;
    int timePosY = marginHeight + mapHeight * cellSize + marginHeight / 5;
    total_time_text.setPosition(timePosX, timePosY);
    total_time_text.setFillColor(sf::Color::White);

    command_text.setFont(open_sans);
    command_text.setString(s_command);
    int cmdPosX = WINDOW_WIDTH / 2 - command_text.getLocalBounds().width / 2;
    int cmdPosY = timePosY + 30;
    command_text.setPosition(cmdPosX, cmdPosY);
    command_text.setFillColor(sf::Color::White);



    Coordinate start = Coordinate(0, 0);
    Coordinate end = Coordinate(mapWidth - 1, mapHeight - 1);
    Map my_map(start, end, cellSize, mapWidth, mapHeight, marginWidth, marginHeight);
    vector<Coordinate> my_path;
    Path pathFinder(my_map, start, end, method);
    
    window.setFramerateLimit(60);
    sf::Clock deltaClock;
    random_device rd;
    mt19937 gen(rd());

    bool search_active = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int mx = sf::Mouse::getPosition(window).x - marginWidth;
            int my = sf::Mouse::getPosition(window).y - marginHeight;
            int x = mx / cellSize;
            int y = my / cellSize;
            if (!(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)) {
                my_map.putWall(Coordinate(x, y));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            my_path.clear();
            pathFinder.initSearch(my_map, start, end);
            search_active = true;
            s_totalTime = 0.0f;
        }
        
        s_deltaTime = deltaClock.restart().asSeconds();
        if (s_deltaTime > s_timeStep)
            s_deltaTime = s_timeStep;
        s_elapsedTime += s_deltaTime;
        while (s_elapsedTime >= s_timeStep) {
                s_elapsedTime -= s_timeStep;
                // ctr++;
                if (search_active == true) {
                    s_totalTime += s_timeStep;
                }
                bool match = pathFinder.floodFill(my_map, start, end, gen);
                if (match == true) {
                    cout<<boolalpha<<"match "<<match<<endl;
                    search_active = false;
                }
                my_path = pathFinder.getPath();
        }

        // render
        window.clear();

        my_map.draw(window, my_path);
        window.draw(title_text);
        window.draw(command_text);
        total_time_text.setString(to_string(s_totalTime));
        window.draw(total_time_text);
        window.display();

    }

    return 0;
}
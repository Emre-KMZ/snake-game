#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <string>

#ifndef SNAKE_ENGINE_HH
#define SNAKE_ENGINE_HH




static const char kSnakeBody = '#';
static const char kFood = '*';
static const char kBorder = (char)219;
static const int kInitialLength = 4;

/*
down 2
left 4
up 3
right 5
*/

/*
Snake Engine
Also responsible for window management and input processing
*/
class SnakeGame{
public:
    void initializeWindow();
    SnakeGame();
    void run();

    
    private:
    struct Coord{
        int row;
        int col;
    };
    
    std::vector<int> dir_row = {0, 1, 0, -1};
    std::vector<int> dir_col = {1, 0, -1, 0};

    std::vector<char> dirToHead = {'>', 'v', '<', '^'};

    std::deque<Coord> snake;
    // 0,0 is top left -- ncurses convention
    // x is column
    // y is row
    int height; // number of rows
    int width; // number of cols
    int curDir; // 0 for right, 1 for down, ...
    Coord foodLoc;
    const int refreshRate = 70000; // microseconds

    void drawBorders();
    void drawInitialSnake();
    bool getDirectionOrQuitNotBlocking();
    void extendSnake();
    void placeFood();
    bool justAteFood();
    void trimTail();
    bool hitsBorderOrItself();
    void finishScreen();
};

#endif // SNAKE_ENGINE_HH


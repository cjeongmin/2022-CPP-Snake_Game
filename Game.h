#ifndef __GAME__
#define __GAME__

#include <ncurses.h>
#include <unistd.h>

#include "Snake.h"

#define MICROSECOND_SECOND 1e6
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

class Game {
   private:
    int stage[21][21];
    Snake snake;
    WINDOW *game, *score, *mission;

   public:
    Game();
    ~Game();
    void setColors();
    void drawWindowBorder(WINDOW *window, int height, int width);
    void drawGameBoard();
    void createItem();
    void createPoison();
    void run();
    void pause();  // for testing
};

#endif
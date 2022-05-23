#ifndef __SNAKE__
#define __SNAKE__

#include <deque>
using namespace std;

#include "Cell.h"
#include "Type.h"

class Snake {
   private:
    deque<Cell> cell;
    int maximum_length;
    int direction;  // 서: 0, 북: 1, 동: 2, 남: 3
    int dx[4];
    int dy[4];

   public:
    Snake();
    ~Snake();
    void setDirection(int d);
    int getDirection() const;
    int size() const;
    int max_size() const;
    int move(int stage[21][21]);
    void show(int stage[21][21]);
};

#endif
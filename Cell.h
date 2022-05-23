#ifndef __CELL__
#define __CELL__

typedef struct Cell {
    int type;
    int r, c;
    Cell(int _r = -1, int _c = -1, int _type = 0) : r(_r), c(_c), type(_type) {}
} Cell;

#endif
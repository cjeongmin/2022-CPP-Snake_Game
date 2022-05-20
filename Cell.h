#ifndef __CELL__
#define __CELL__

typedef struct Cell {
    int type;
    int r, c;
    Cell(int _r, int _c, int _type) : r(_r), c(_c), type(_type) {}
} Cell;

#endif
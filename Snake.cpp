#include "Snake.h"

Snake::Snake() {
    direction = 0;
    dx[0] = dy[1] = -1;
    dx[2] = dy[3] = 1;
    dx[1] = dx[3] = dy[0] = dy[2] = 0;

    maximum_length = 3;
    cell.push_back(Cell(10, 10, HEAD));
    cell.push_back(Cell(10, 11, BODY));
    cell.push_back(Cell(10, 12, BODY));
}

Snake::~Snake() {}

void Snake::setDirection(int d) {
    direction = d;
}

int Snake::getDirection() const {
    return direction;
}

int Snake::size() const {
    return cell.size();
}

int Snake::max_size() const {
    return maximum_length;
}

int Snake::move(int stage[21][21]) {
    int x = cell.front().c + dx[direction];
    int y = cell.front().r + dy[direction];

    if (stage[y][x] == WALL || stage[y][x] == BODY || stage[y][x] == IMMUNE_WALL) {
        return 0;
    }

    cell.front().type = BODY;
    if (stage[y][x] == ITEM) {
        cell.push_front(Cell(y, x, HEAD));
        if (cell.size() > maximum_length) maximum_length = cell.size();
        return ITEM;
    } else {
        cell.push_front(Cell(y, x, HEAD));
        cell.pop_back();

        if (stage[y][x] == POISON) {
            cell.pop_back();
            return POISON;
        }
    }

    return 1;
}

void Snake::show(int stage[21][21]) {
    for (int y = 0; y < 21; y++)
        for (int x = 0; x < 21; x++)
            if (stage[y][x] == HEAD || stage[y][x] == BODY) stage[y][x] = EMPTY;

    for (int length = cell.size(); length; length--) {
        Cell c = cell.front();
        cell.pop_front();
        stage[c.r][c.c] = (c.type == HEAD ? HEAD : BODY);
        cell.push_back(c);
    }
}
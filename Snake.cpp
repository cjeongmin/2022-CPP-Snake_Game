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
    } else if (stage[y][x] == GATE) {
        int r, c;
        for (int i = 0, flag = true; flag && i < 21; i++) {
            for (int j = 0; j < 21; j++) {
                if (i != y && j != x && stage[i][j] == GATE) {
                    r = i;
                    c = j;
                    flag = false;
                    break;
                }
            }
        }

        // 1) 진행 방향 2) 시계 방향 3) 역시계 방향 4) 반대 방향
        int nx1 = c + dx[direction], ny1 = r + dy[direction];
        int nx2 = c + dx[(direction + 1) % 4], ny2 = r + dy[(direction + 1) % 4];
        int nx3 = c + dx[(direction + 3) % 4], ny3 = r + dy[(direction + 3) % 4];
        int nx4 = c + dx[(direction + 2) % 4], ny4 = r + dy[(direction + 2) % 4];
        if (inRange(ny1, nx1) && !isBlocked(stage, ny1, nx1)) {
            cell.push_front(Cell(ny1, nx1, HEAD));
        } else if (inRange(ny2, nx2) && !isBlocked(stage, ny2, nx2)) {
            cell.push_front(Cell(ny2, nx2, HEAD));
            direction = (direction + 1) % 4;
        } else if (inRange(ny3, nx3) && !isBlocked(stage, ny3, nx3)) {
            cell.push_front(Cell(ny3, nx3, HEAD));
            direction = (direction + 3) % 4;
        } else if (inRange(ny4, nx4) && !isBlocked(stage, ny4, nx4)) {
            cell.push_front(Cell(ny4, nx4, HEAD));
            direction = (direction + 2) % 4;
        }
        cell.pop_back();
        return GATE;
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

void Snake::init() {
    cell.clear();
    maximum_length = 3;
    cell.push_back(Cell(10, 10, HEAD));
    cell.push_back(Cell(10, 11, BODY));
    cell.push_back(Cell(10, 12, BODY));
    direction = 0;
}
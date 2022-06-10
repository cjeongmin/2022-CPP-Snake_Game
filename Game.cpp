#include "Game.h"

#include <iostream>

Game::Game() {
    srand(time(NULL));
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 21; j++) {
            if (i == 0 || i == 20 || j == 0 || j == 20) {
                stage[i][j] = WALL;
            } else {
                stage[i][j] = EMPTY;
            }
        }
    }
    stage[0][0] = stage[0][20] = stage[20][0] = stage[20][20] = IMMUNE_WALL;

    // stage 0
    // for (int i = 5; i < 16; i++) {
    //     for (int j = 5; j < 16; j++) {
    //         if (i == 10 || j == 10) continue;
    //         if (i == 5 || i == 15 || j == 5 || j == 15) {
    //             stage[i][j] = WALL;
    //         }
    //     }
    // }

    // stage 1
    // for (int i = 5; i < 16; i++) {
    //     for (int j = 5; j < 16; j++) {
    //         if (i == 5 || i == 15 || j == 5 || j == 15) {
    //             stage[i][j] = WALL;
    //         }
    //     }
    // }

    // stage 2
    // for (int i = 0; i < 21; i++) {
    //     for (int j = 0; j < 21; j++) {
    //         if (i == 10 || j == 10) {
    //             stage[i][j] = WALL;
    //         }
    //     }
    // }
    
    // stage 3
    // for (int i = 0; i < 16; i++)
    //     stage[6][i] = WALL;
    // stage[6][0] = IMMUNE_WALL; 
    // for (int i = 5; i < 21; i++)
    //     stage[14][i] = WALL;
    // stage[14][20] = IMMUNE_WALL;

    // stage 4
    for (int i = 1; i < 8; i++)
        stage[4][i] = WALL;
    stage[4][0] = IMMUNE_WALL;
    for (int i = 13; i < 21; i++)
        stage[16][i] = WALL;
    stage[16][20] = IMMUNE_WALL;
    for (int i = 4; i < 17; i++)
        stage[i][7] = WALL;
    for (int i = 4; i < 17; i++)
        stage[i][13] = WALL;
    

    initscr();
    resize_term(23, 80);
    curs_set(0);
    nodelay(stdscr, true);
    noecho();
    keypad(stdscr, true);
    cbreak();
    refresh();
    game = newwin(23, 46, 0, 0);
    score = newwin(11, 34, 0, 46);
    mission = newwin(13, 34, 10, 46);
    elapsed = itemCount = poisonCount = 0;
}

Game::~Game() {
    endwin();
}

void Game::setColors() {
    // 기본 색상 설정
    start_color();

    init_color(BORDER, 250, 250, 250);
    init_pair(BORDER, BORDER, BORDER);

    init_color(WALL, 450, 450, 450);
    init_pair(WALL, WALL, WALL);

    init_color(IMMUNE_WALL, 450, 450, 450);
    init_pair(IMMUNE_WALL, IMMUNE_WALL, IMMUNE_WALL);

    init_color(EMPTY, 1000, 1000, 1000);
    init_pair(EMPTY, EMPTY, EMPTY);

    init_color(HEAD, 0, 250, 0);
    init_pair(HEAD, HEAD, HEAD);

    init_color(BODY, 0, 500, 0);
    init_pair(BODY, BODY, BODY);

    init_color(ITEM, 0, 0, 1000);
    init_pair(ITEM, ITEM, ITEM);

    init_color(POISON, 1000, 0, 0);
    init_pair(POISON, POISON, POISON);

    init_color(GATE, 500, 500, 0);
    init_pair(GATE, GATE, GATE);

    init_pair(TEXT, COLOR_BLACK, EMPTY);
}

void Game::drawWindowBorder(WINDOW *window, int height, int width) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width / 2; c++) {
            if (r == 0 || r == height - 1 || c == 0 || c == width / 2 - 1) {
                wattron(window, COLOR_PAIR(BORDER));
                mvwprintw(window, r, c * 2, "  ");
                wattroff(window, COLOR_PAIR(BORDER));
            } else {
                wattron(window, COLOR_PAIR(EMPTY));
                mvwprintw(window, r, c * 2, "  ");
                wattroff(window, COLOR_PAIR(EMPTY));
            }
        }
    }
    wrefresh(window);
}

void Game::drawGameBoard() {
    snake.show(stage);

    for (int r = 0; r < 21; r++) {
        for (int c = 0; c < 21; c++) {
            wattron(game, COLOR_PAIR(stage[r][c]));
            mvwprintw(game, r + 1, (c + 1) * 2, "  ");
            wattroff(game, COLOR_PAIR(stage[r][c]));
        }
    }

    wrefresh(game);
}

void Game::drawScoreBoard() {
    wattron(score, COLOR_PAIR(TEXT) | A_BOLD);
    mvwprintw(score, 1, 12, "SCORE BOARD");
    wattroff(score, COLOR_PAIR(TEXT) | A_BOLD);

    wattron(score, COLOR_PAIR(TEXT));
    mvwprintw(score, 3, 6, "B: %d / %d", snake.size(), snake.max_size());
    mvwprintw(score, 4, 6, "+: %d", itemCount);
    mvwprintw(score, 5, 6, "-: %d", poisonCount);
    mvwprintw(score, 6, 6, "G: %d", gateCount);
    mvwprintw(score, 7, 6, "elapsed: %ds", elapsed / 2);
    wattroff(score, COLOR_PAIR(TEXT));

    wrefresh(score);
}

void Game::drawMissionBoard() {
    wattron(mission, COLOR_PAIR(TEXT) | A_BOLD);
    mvwprintw(mission, 1, 14, "MISSION");
    wattroff(mission, COLOR_PAIR(TEXT) | A_BOLD);

    wattron(mission, COLOR_PAIR(TEXT));
    mvwprintw(mission, 3, 6, "+(3): ( %s )", (itemCount >= 3 ? "v" : "x"));
    mvwprintw(mission, 4, 6, "asd: ( %s )", "x");
    wattroff(mission, COLOR_PAIR(TEXT));

    wrefresh(mission);
}

void Game::create(TYPE type) {
    Cell &item = items[type - ITEM];
    if (item.type) {
        stage[item.r][item.c] = EMPTY;
    }

    while (true) {
        int r = rand() % 20 + 1;
        int c = rand() % 20 + 1;

        if (stage[r][c] == EMPTY) {
            stage[r][c] = type;
            item.r = r;
            item.c = c;
            item.type = type;
            return;
        }
    }
}

void Game::generateGate() {
    for (int i = 0; i < 2; i++) {
        while (true) {
            int r = rand() % 20 + 1;
            int c = rand() % 20 + 1;

            if (stage[r][c] == WALL) {
                stage[r][c] = GATE;
                break;
            }
        }
    }
}

void Game::run() {
    drawWindowBorder(game, 23, 46);
    drawWindowBorder(score, 11, 34);
    drawWindowBorder(mission, 13, 34);
    drawGameBoard();
    drawScoreBoard();
    drawMissionBoard();
    generateGate();

    int itemCount = 0;
    int poisonCount = 0;
    while (true) {
        int key = getch();
        if (key == 'q') {
            break;
        } else if (key == KEY_DOWN && snake.getDirection() != UP) {  // 아래
            snake.setDirection(DOWN);
        } else if (key == KEY_UP && snake.getDirection() != DOWN) {  // 위
            snake.setDirection(UP);
        } else if (key == KEY_LEFT && snake.getDirection() != RIGHT) {  // 왼쪽
            snake.setDirection(LEFT);
        } else if (key == KEY_RIGHT && snake.getDirection() != LEFT) {  // 오른쪽
            snake.setDirection(RIGHT);
        } else {
            // 다른 키 입력시 문제 여기로
        }

        int move_result = snake.move(stage);
        if (!move_result || snake.size() < 3) {
            break;
        } else {
            if (move_result == ITEM || !(itemCount % 20)) {
                create(ITEM);
                itemCount = 0;
                if (move_result == ITEM) this->itemCount++;
            }
            if (move_result == POISON || !(poisonCount % 25)) {
                create(POISON);
                poisonCount = 0;
                if (move_result == POISON) this->poisonCount++;
            }
            if (move_result == GATE) {
                this->gateCount++;
            }
        }

        itemCount++;
        poisonCount++;
        elapsed++;

        drawGameBoard();
        drawScoreBoard();
        drawMissionBoard();

        usleep(MICROSECOND_SECOND * 0.5);
    }

    nodelay(stdscr, false);
    getch();
}

void Game::pause() {
    getch();
}

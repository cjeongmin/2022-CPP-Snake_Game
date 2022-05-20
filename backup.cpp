#include <ncurses.h>
#include <unistd.h>
#define MICROSECOND_SECOND 1e6

void printBox(int pair, int y, int x, WINDOW *win) {
    wattron(win, COLOR_PAIR(pair));
    mvwprintw(win, y, x * 2, "  ");
    wattroff(win, COLOR_PAIR(pair));
}

int main() {
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    noecho();

    while (1) {
        char c = getch();
        if (c == 'A') {
            break;
        } else if (c == ERR) {
            printw("ERR\n");
        } else {
            printw("%d %c\n", c, c);
        }
        refresh();
        usleep(MICROSECOND_SECOND * 0.5);
    }

    endwin();

    /* 박스 그리는 예제 */
    // start_color();
    // init_pair(1, COLOR_BLUE, COLOR_BLUE);
    // init_pair(2, COLOR_RED, COLOR_RED);

    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         if (i == 0 || i == 9 || j == 0 || j == 9) {
    //             printBox(2, i, j, stdscr);
    //         } else {
    //             printBox(1, i, j, stdscr);
    //         }
    //         refresh();
    //         getch();
    //     }
    // }

    // printw("END");
    // refresh();
    // getch();
    return 0;
}
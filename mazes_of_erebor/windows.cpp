
#include <ncurses.h>

// global variables
const int WINDOW_WIDTH(80);
const int WINDOW_HEIGHT(22);
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);
const int GAME_WIDTH(73);  // TODO: Combine with master window dimensions?
const int GAME_HEIGHT(1 + GAME_WIDTH / 2);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win);
void init_maze_window(WINDOW *win);

// TODO: ON RESIZE OF MAZE, GAME FAILS!!!!!!!!!!!!!


/**
 *
 */
WINDOW* init_window() {
    // init NCURSES GUI
    initscr();
    clear();  // TODO: Necessary?
    noecho();              // disable echo text
    cbreak();              // disable line buffering
    curs_set(false);       // disable cursor
    keypad(stdscr, true);  // enable key pad
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, COLOR_RED, COLOR_RED);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    return newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
}


/**
 *
 */
void init_menu_window(WINDOW *win) {
    int startx(0);
    int starty(0);

    startx = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    starty = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    *win = *newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
    keypad(win, TRUE);

    clear();
}


/**
 *
 */
void init_maze_window(WINDOW *win) {
    *win = *newwin(GAME_HEIGHT, GAME_WIDTH, 0, 0);  // TODO: Testing
    box(win, 0, 0);
}


#include <ncurses.h>

// global variables
const int WINDOW_WIDTH(73);
const int WINDOW_HEIGHT(1 + WINDOW_WIDTH / 2);
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win);
void init_maze_window(WINDOW *win);


/**
 *   NCURSES initializer at program start
 */
WINDOW* init_window() {
    // init NCURSES GUI
    initscr();
    clear();
    noecho();              // disable echo text
    cbreak();              // disable line buffering
    curs_set(false);       // disable cursor
    keypad(stdscr, true);  // enable key pad

    // init color scheme
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_BLACK);    // background
    init_pair(2, COLOR_RED, COLOR_WHITE);      // player on medium & hard
    init_pair(3, COLOR_WHITE, COLOR_WHITE);    // walls on easy, torch adj on hard
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);  // torch close on hard
    init_pair(5, COLOR_RED, COLOR_RED);        // torch far on hard, path on medium
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);   // player on easy

    return newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
}


/**
 *   NCURSES initializer for a menu
 */
void init_menu_window(WINDOW *win) {
    int startx(0);
    int starty(0);

    startx = (WINDOW_WIDTH - MENU_WIDTH) / 2;
    starty = (WINDOW_HEIGHT - MENU_HEIGHT) / 4;

    *win = *newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
    keypad(win, TRUE);

    clear();
}


/**
 *   NCURSES initializer for the actual mazes
 */
void init_maze_window(WINDOW *win) {
    *win = *newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    box(win, 0, 0);
}

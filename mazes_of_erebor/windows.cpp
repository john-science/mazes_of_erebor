
#include <ncurses.h>

// global variables
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win, int win_y, int win_x);
void init_maze_window(WINDOW *win, int& win_y, int& win_x);


/**
 *   NCURSES initializer at program start
 */
WINDOW* init_window() {
    int win_x, win_y;

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

    getmaxyx(stdscr, win_y, win_x);
    return newwin(win_y, win_x, 0, 0);
}


/**
 *   NCURSES initializer for a menu
 */
void init_menu_window(WINDOW *win, int win_y, int win_x) {
    int height(MENU_HEIGHT > win_y ? MENU_HEIGHT : win_y);
    int width(MENU_WIDTH > win_x ? MENU_WIDTH : win_x);

    int starty(0);
    int startx(0);
    starty = (height - MENU_HEIGHT) / 2;
    startx = (width - MENU_WIDTH) / 2;

    *win = *newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
    keypad(win, TRUE);

    clear();
}


/**
 *   NCURSES initializer for the actual mazes
 */
void init_maze_window(WINDOW *win) {
    int win_y, win_x;
    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
    box(win, 0, 0);
}

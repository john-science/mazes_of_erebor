
#include <ncurses.h>

const int MIN_WINDOW_SIZE(13);

// forward declaration
WINDOW* init_window();
void init_maze_window(WINDOW *win, int& win_y, int& win_x);
void full_box_clear(WINDOW *win);
void full_box_resize(WINDOW *win, int win_y, int win_x);


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
 *   NCURSES initializer for the actual mazes
 */
void init_maze_window(WINDOW *win) {
    int win_y, win_x;
    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
}


/**
 *  Clear the window completely and add a box around it.
 */
void full_box_clear(WINDOW *win) {
    wclear(win);
    clear();
    refresh();
    wrefresh(win);
}


/**
 *  Resize the window and add a box around it.
 */
void full_box_resize(WINDOW *win, int win_y, int win_x) {
    wresize(win, win_y, win_x);
    wclear(win);
    refresh();
    wrefresh(win);
}

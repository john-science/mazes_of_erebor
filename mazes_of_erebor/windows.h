
#ifndef WINDOWS_H
#define WINDOWS_H

// global variables
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win, int win_y, int win_x);
void init_maze_window(WINDOW *win);

#endif

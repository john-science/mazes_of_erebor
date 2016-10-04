
#ifndef WINDOWS_H
#define WINDOWS_H

// global variables
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);

// forward declaration
WINDOW* init_window();
void init_maze_window(WINDOW *win);
void full_box_clear(WINDOW *win);
void full_box_resize(WINDOW *win, int win_y, int win_x);

#endif

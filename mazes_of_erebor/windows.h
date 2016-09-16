
#ifndef WINDOWS_H
#define WINDOWS_H

// global variables
const int WINDOW_WIDTH(73);
const int WINDOW_HEIGHT(1 + WINDOW_WIDTH / 2);
const int MENU_WIDTH(30);
const int MENU_HEIGHT(10);

// forward declaration
WINDOW* init_window();
void init_menu_window(WINDOW *win);
void init_maze_window(WINDOW *win);

#endif


#ifndef MAZE_WIN_H
#define MAZE_WIN_H

#include "data.h"

void maze_print_easy(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_medium(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_hard(WINDOW *win, const maze_data maze, const player_data player);
menu_state game_loop_maze(WINDOW *win, game_data *d, menu_state state);

#endif


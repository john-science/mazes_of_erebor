
#ifndef MAZE_WIN_H
#define MAZE_WIN_H

#include "data.h"

void maze_print_easy(WINDOW*, const GameData);
void maze_print_medium(WINDOW*, const GameData);
void maze_print_hard(WINDOW*, const GameData);
menu_state game_loop_maze(WINDOW *win, GameData *d, menu_state state);

#endif


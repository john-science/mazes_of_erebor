
#ifndef GAME_H
#define GAME_H
#include "data.h"
#include "maze.h"
#include "menu.h"

void maze_loop_update(const int c, bool *needs_update, maze_data *maze, player_data *player);
void maze_loop_init(maze_data *maze, player_data *player, const menu_state state);  // TODO: Remove menu state reference

#endif

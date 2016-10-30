
#ifndef GAME_H
#define GAME_H

#include "data.h"
#include "maze.h"

void maze_loop_update(const int c, bool *needs_update, GameData *game);
void maze_loop_init(GameData *game, const menu_state state);

#endif

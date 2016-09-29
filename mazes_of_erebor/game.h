
#ifndef GAME_H
#define GAME_H
#include "data.h"
#include "maze.h"
#include "menu.h"

menu_state game_ui(WINDOW *menu_win, game_data *d, menu_state state);

#endif


#ifndef DATA_H
#define DATA_H

#include <string>
#include "player.h"
#include "maze.h"

using namespace std;


enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


class GameData {
public:
    GameData();
    Maze maze;
    PlayerData player;
    menu_state difficulty;

    void restart_level();
};


#endif

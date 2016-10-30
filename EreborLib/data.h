
#ifndef DATA_H
#define DATA_H

#include "player.h"
#include "maze.h"

using namespace std;


// menu state to be used by GUI
enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 game_win, quit};


class GameData {
public:
    GameData();
    Maze maze;
    PlayerData player;
    menu_state difficulty;

    void restart_level();
    bool reached_end();
private:
    int win_level;
};


#endif

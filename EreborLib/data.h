
#ifndef DATA_H
#define DATA_H

#include <string>
#include "maze.h"

using namespace std;


// TODO: Does this belong in data, or in menu?
enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


enum game_state {game_intro, game_play, game_leveled};


class PlayerData {
public:
    PlayerData();
    int loc[2];
    string name;
    string parent1;  // dwarf
    string parent2;  // human
    bool *visited;
};


class GameData {
public:
    GameData();
    Maze maze;
    PlayerData player;
    menu_state difficulty;

    void restart_level();
};


#endif

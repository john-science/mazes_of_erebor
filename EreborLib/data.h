
#ifndef DATA_H
#define DATA_H

#include <string>

using namespace std;


const int MAX_MAZE_SIZE(71);


// TODO: Does this belong in data, or in menu?
enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


enum game_state {game_intro, game_play, game_leveled};


class MazeData {
public:
    MazeData();  // standard initializer
    int nrows;
    int ncols;
    bool *grid;
    int level;
    menu_state difficulty = game_easy;
    int start[2];
    int finish[2];
};


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
    MazeData maze;
    PlayerData player;

    void restart_level();
};


#endif

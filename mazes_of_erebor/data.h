
#ifndef DATA_H
#define DATA_H

#include <string>

using namespace std;


const int MIN_WINDOW_SIZE(15);
const int MAX_MAZE_SIZE(71);


enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


struct maze_data {
    int nrows = 19;
    int ncols = 31;
    bool *grid = new bool[nrows * ncols];
    int level = -1;
    menu_state difficulty = game_easy;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};
};


struct player_data {
    int loc[2] = {1, 1};
    string name = "Khorin";
    string parent1 = "Balin";  // dwarf
    string parent2 = "Rogyr";  // human
    bool *visited = new bool[1];
};


struct game_data {
    maze_data maze;
    player_data player;
};


void reset_player(player_data *player, maze_data *maze);

#endif


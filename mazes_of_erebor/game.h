
#ifndef GAME_H
#define GAME_H
#include "maze.h"

struct player_data {
    int position[2] = {1, 1};
    std::string nomen = "Khorin";
    std::string parent1 = "Balin";  // dwarf
    std::string parent2 = "Rogyr";  // human
    bool visited[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
};

struct game_data {
    maze_data maze;
    player_data player;
};

menu_state game_ui(WINDOW *menu_win, menu_state state);

#endif


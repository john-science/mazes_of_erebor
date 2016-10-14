

#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "data.h"
#include "maze.h"
#include "menu.h"

using namespace std;

// forward declarations
void maze_loop_update(const int c, bool *needs_update, maze_data *maze, player_data *player);
void maze_loop_init(maze_data *maze, player_data *player, const menu_state state);  // TODO: Remove menu state reference


/**
 *  Initialize the data, for a fresh game.
 */
void maze_loop_init(maze_data *maze, player_data *player, const menu_state state) {
    maze->level = 0;
    gen_new_maze(maze);
    reset_player(player, maze);
    maze->difficulty = state;
    player->visited[maze->start[0] * maze->ncols + maze->start[1]] = true;
}


/**
 *  Master UPDATE for game loop - in maze
 */
void maze_loop_update(const int c, bool *needs_update, maze_data *maze, player_data *player) {
    // UPDATE 1: direct updates
    switch (c) {
        case KEY_UP:
            if (maze_valid_move(*maze, player->loc[0] - 1, player->loc[1])) {
                player->loc[0] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_DOWN:
            if (maze_valid_move(*maze, player->loc[0] + 1, player->loc[1])) {
                player->loc[0] += 1;
                *needs_update = true;
            }
            break;
        case KEY_LEFT:
            if (maze_valid_move(*maze, player->loc[0], player->loc[1] - 1)) {
                player->loc[1] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_RIGHT:
            if (maze_valid_move(*maze, player->loc[0], player->loc[1] + 1)) {
                player->loc[1] += 1;
                *needs_update = true;
            }
            break;
        // no default actions to be taken
    }

    // UPDATE 2: indirect updates
    if (needs_update) {
        // if you reach the end, start over in a new maze
        if (player->loc[0] == maze->finish[0] && player->loc[1] == maze->finish[1]) {
            gen_new_maze(maze);
            reset_player(player, maze);
            maze->level += 1;
        }

        player->visited[player->loc[0] * maze->ncols + player->loc[1]] = true;
    }
}


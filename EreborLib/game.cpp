

#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "data.h"
#include "maze.h"

using namespace std;

// forward declarations
void maze_loop_update(const int c, bool *needs_update, GameData *game);
void maze_loop_init(GameData *game, const menu_state state);  // TODO: Remove menu state reference


/**
 *  Initialize the data, for a fresh game.
 */
void maze_loop_init(GameData *game, const menu_state state) {
    game->maze.level = 0;
    gen_new_maze(&game->maze);
    game->restart_level();
    game->maze.difficulty = state;
    game->player.visited[game->maze.start[0] * game->maze.ncols + game->maze.start[1]] = true;
}


/**
 *  Master UPDATE for game loop - in maze
 */
void maze_loop_update(const int c, bool *needs_update, GameData *game) {
    // UPDATE 1: direct updates
    switch (c) {
        case KEY_UP:
            if (maze_valid_move(game->maze, game->player.loc[0] - 1, game->player.loc[1])) {
                game->player.loc[0] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_DOWN:
            if (maze_valid_move(game->maze, game->player.loc[0] + 1, game->player.loc[1])) {
                game->player.loc[0] += 1;
                *needs_update = true;
            }
            break;
        case KEY_LEFT:
            if (maze_valid_move(game->maze, game->player.loc[0], game->player.loc[1] - 1)) {
                game->player.loc[1] -= 1;
                *needs_update = true;
            }
            break;
        case KEY_RIGHT:
            if (maze_valid_move(game->maze, game->player.loc[0], game->player.loc[1] + 1)) {
                game->player.loc[1] += 1;
                *needs_update = true;
            }
            break;
        // no default actions to be taken
    }

    // UPDATE 2: indirect updates
    if (needs_update) {
        // if you reach the end, start over in a new maze
        if (game->player.loc[0] == game->maze.finish[0] && game->player.loc[1] == game->maze.finish[1]) {
            gen_new_maze(&game->maze);
            game->restart_level();
            game->maze.level += 1;
        }

        game->player.visited[game->player.loc[0] * game->maze.ncols + game->player.loc[1]] = true;
    }
}


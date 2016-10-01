
#include <functional>
#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "data.h"
#include "maze.h"
#include "menu.h"
#include "story.h"
#include "windows.h"

using namespace std;


/**
 *   The game maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
menu_state game_ui(WINDOW *win, game_data *d, menu_state state)
{
    maze_data *maze = &d->maze;
    player_data *player = &d->player;
    int c;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    // init window at current resolution
    if (maze->level == -1) {
        intro_splash(win);
    }
    init_maze_window(win);
    getmaxyx(stdscr, win_y, win_x);
    last_win_y = win_y;
    last_win_x = win_x;

    // generate a new maze, if necessary
    if (maze->level == -1) {
        maze->level = 0;
        gen_new_maze(maze);
        reset_player(player, maze);
        maze->difficulty = state;
    }

    // select the appropriate print function
    function<void(WINDOW*, const maze_data, const player_data)> maze_print = \
        state == game_easy ? maze_print_easy : (state == game_medium ? maze_print_medium : maze_print_hard);

    // game loop
    while (true) {
        player->visited[player->loc[0] * maze->max_size + player->loc[1]] = true;
        maze_print(win, *maze, *player);

        // input and update
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(*maze, player->loc[0] - 1, player->loc[1])) {
                    player->loc[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(*maze, player->loc[0] + 1, player->loc[1])) {
                    player->loc[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(*maze, player->loc[0], player->loc[1] - 1)) {
                    player->loc[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(*maze, player->loc[0], player->loc[1] + 1)) {
                    player->loc[1] += 1;
                }
                break;
            case 113:  // q
                return menu_cont;
            case KEY_RESIZE:
                getmaxyx(stdscr, win_y, win_x);
                if (last_win_x != win_x || last_win_y != win_y) {
                    last_win_y = win_y;
                    last_win_x = win_x;
                    full_box_resize(win, win_y, win_x);
                }
                break;
            // no default actions to be taken
        }

        // If you reach the end, start over in a new maze
        if (player->loc[0] == maze->finish[0] && player->loc[1] == maze->finish[1]) {
            success_splash(win, maze->level + 2);
            gen_new_maze(maze);
            reset_player(player, maze);
            maze->level += 1;
        }
    }
}

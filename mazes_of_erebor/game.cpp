
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


// forward declarations
menu_state game_ui(WINDOW *menu_win, game_data *d, menu_state state);
void get_new_dims(int& nrows, int& ncols, int level);


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
        // TODO: Move to data.cpp
        backtracking_maze_gen(maze);
        gen_entrances_opposites(maze);
        std::fill_n(player->visited, maze->max_size * maze->max_size / 2, false);
        player->visited[maze->finish[0] * maze->max_size + maze->finish[1]] = true;
        player->loc[0] = maze->start[0];
        player->loc[1] = maze->start[1];
        maze->level = 0;
        maze->difficulty = state;
    }

    // game loop
    while (true) {
        player->visited[player->loc[0] * maze->max_size + player->loc[1]] = true;

        // TODO: load this from generic template earlier, to save the ifs
        if (state == game_easy) {
            maze_print_easy(win, *maze, player->loc);
        } else if (state == game_medium) {
            maze_print_medium(win, *maze, player->visited, player->loc);
        } else if (state == game_hard) {
            maze_print_hard(win, *maze, player->loc);
        }

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
                    wresize(win, win_y, win_x);
                    wclear(win);
                    box(win, 0, 0);
                    refresh();
                    wrefresh(win);
                }
                break;
            // no default actions to be taken
        }

        // TODO: How to check for value equality in arrays?
        // If you reach the end, start over in a new maze
        if (player->loc[0] == maze->finish[0] && player->loc[1] == maze->finish[1]) {
            success_splash(win, maze->level + 2);
            wclear(win);
            clear();
            box(win, 0, 0);
            refresh();
            wrefresh(win);
            get_new_dims(maze->nrows, maze->ncols, maze->level);

            // generate a new maze
            std::fill_n(player->visited, maze->max_size * maze->max_size / 2, false);
            backtracking_maze_gen(maze);
            gen_entrances_opposites(maze);
            player->loc[0] = maze->start[0];
            player->loc[1] = maze->start[1];

            maze->level += 1;
        }
    }
}


/**
 *   Randomly generate maze dimensions.
 */
void get_new_dims(int& nrows, int& ncols, int level) {
    level %= 20;

    const int bottom_y = 15;
    nrows = bottom_y + level / 2 + (rand() % (int)(level / 2 + 1));
    if (nrows % 2 == 0) { nrows += 1; }

    const int bottom_x = 31;
    ncols = bottom_x + level + (rand() % (int)((level) + 1));
    if (ncols % 2 == 0) { ncols += 1; }
}

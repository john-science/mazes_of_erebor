
#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "maze.h"
#include "menu.h"
#include "story.h"
#include "windows.h"

using namespace std;

// forward declarations
game_state game_ui(WINDOW *menu_win, game_state state);
game_state game_ui_medium(WINDOW *menu_win);
void get_new_dims(int& nrows, int& ncols, int count);


/**
 *   The basic maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
game_state game_ui(WINDOW *win, game_state state)
{
    maze_data maze;
    int player[2] = {1, 1};
    int count(0);
    int c;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    // init window at current resolution
    intro_splash(win);
    init_maze_window(win);
    getmaxyx(stdscr, win_y, win_x);
    last_win_y = win_y;
    last_win_x = win_x;

    // generate a new maze
    backtracking_maze_gen(&maze);
    gen_entrances_opposites(&maze);
    player[0] = maze.start[0];
    player[1] = maze.start[1];

    while (true) {
        if (state == game_easy) {
            maze_print_easy(win, maze, player);
        } else if (state == game_hard) {
            maze_print_hard(win, maze, player);
        }

        // input and update
        // TODO: Am I wasting a LOT of cycles here?
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, player[0], player[1] + 1)) {
                    player[1] += 1;
                }
                break;
            case 113:  // q
                return menu_main;
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

        // If you reach the end, start over in a new maze
        if (player[0] == maze.finish[0] && player[1] == maze.finish[1]) {
            success_splash(win, count + 2);
            wclear(win);
            clear();
            box(win, 0, 0);
            refresh();
            wrefresh(win);
            get_new_dims(maze.nrows, maze.ncols, count);

            // generate a new maze
            backtracking_maze_gen(&maze);
            gen_entrances_opposites(&maze);
            player[0] = maze.start[0];
            player[1] = maze.start[1];

            count += 1;
        }
    }
}


/**
 *   The basic maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
game_state game_ui_medium(WINDOW *win)
{
    maze_data maze;
    bool visited[maze.max_size * maze.max_size / 2];
    int player[2] = {1, 1};
    int count(0);
    int c;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    // init window at current resolution
    intro_splash(win);
    init_maze_window(win);
    getmaxyx(stdscr, win_y, win_x);
    last_win_y = win_y;
    last_win_x = win_x;

    // generate a new maze
    std::fill_n(visited, maze.max_size * maze.max_size / 2, false);
    backtracking_maze_gen(&maze);
    gen_entrances_opposites(&maze);
    player[0] = maze.start[0];
    player[1] = maze.start[1];

    while (true) {
        visited[player[0] * maze.max_size + player[1]] = true;
        visited[maze.finish[0] * maze.max_size + maze.finish[1]] = true;
        maze_print_medium(win, maze, visited, player);

        // input and update
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, player[0], player[1] + 1)) {
                    player[1] += 1;
                }
                break;
            case 113:  // q
                return menu_main;
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

        // if you reach the end, start over in a new maze
        if (player[0] == maze.finish[0] && player[1] == maze.finish[1]) {
            success_splash(win, count + 2);
            wclear(win);
            clear();
            box(win, 0, 0);
            refresh();
            wrefresh(win);
            get_new_dims(maze.nrows, maze.ncols, count);

            // generate a new maze
            std::fill_n(visited, maze.max_size * maze.max_size / 2, false);
            backtracking_maze_gen(&maze);
            gen_entrances_opposites(&maze);
            player[0] = maze.start[0];
            player[1] = maze.start[1];

            count += 1;
        }
    }
}


/**
 *   Randomly generate maze dimensions.
 */
void get_new_dims(int& nrows, int& ncols, int count) {
    count %= 20;

    const int bottom_y = 15;
    nrows = bottom_y + count / 2 + (rand() % (int)(count / 2 + 1));
    if (nrows % 2 == 0) { nrows += 1; }

    const int bottom_x = 31;
    ncols = bottom_x + count + (rand() % (int)((count) + 1));
    if (ncols % 2 == 0) { ncols += 1; }
}

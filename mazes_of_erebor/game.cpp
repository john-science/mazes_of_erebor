
#include <random>
#include <ncurses.h>
#include <string.h>
#include "maze.h"
#include "menu.h"
#include "windows.h"

// forward declarations
void success_splash(int count);
void get_new_dims(int& nrows, int& ncols, int count);
game_state game_ui(WINDOW *menu_win, game_state state);
game_state game_ui_medium(WINDOW *menu_win);

// constants
const int MAX_MAZE_SIZE(71);

// constants for splash screen
const char* splash_exclaim[] = {"", "Success! ", "Finally! ", "Whew! "};
const int n_splash_exclaim = sizeof(splash_exclaim) / sizeof(char *);
const char* splash_success[] = {"You did it!",
                                "You solved it!",
                                "You solved the maze!",
                                "You found a way out!",
                                "You are through the maze!",
                                "You found your way through the maze!",
                                "You are through!"
                                "You found the end of the laybrinth!",};
const int n_splash_success = sizeof(splash_success) / sizeof(char *);
// TODO: Add more of these
const char* splash_story[] = {"You kick over a dusty old pile of Orcish remains that\n block the staircase.",
                              "You take a short rest before taking the staircase down.",
                              "At the end of the maze you find a staircase leading down.",
                              "You find a staircase leading down and follow it.",
                              "You delve deeper.",
                              "Deeper and deeper into the Halls of the Mountain King...",
                              "You find a curving ramp leading further down into the mountain.",
                              "You find a narrow staircase leading down into the mountain.",
                              "How deep under the mountain does these tunnels go?",
                              "Above the stone doorway you find an engraved\n scene of a human archer killing a dragon.",
                              "Engraved along the walls of the spiral staircase\n are scenes of a dwarf being buried with a glowing gem."};
const int n_splash_story = sizeof(splash_story) / sizeof(char *);


/**
 *   The basic maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
game_state game_ui(WINDOW *win, game_state state)
{
    bool maze[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
    int nrows(19);
    int ncols(31);
    int player[2] = {1, 1};
    int finish[2] = {1, 1};
    int count(0);
    int c;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;
    //init_maze_window(win);
    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
    box(win, 0, 0);
    last_win_y = win_y;
    last_win_x = win_x;

    // generate a new maze
    backtracking_maze_gen(maze, MAX_MAZE_SIZE, nrows, ncols);
    gen_entrances_opposites(maze, MAX_MAZE_SIZE, player, finish, nrows, ncols);

    while (true) {
        if (state == game_easy) {
            maze_print_easy(win, maze, MAX_MAZE_SIZE, nrows, ncols, player, finish);
        } else if (state == game_hard) {
            maze_print_hard(win, maze, MAX_MAZE_SIZE, nrows, ncols, player, finish);
        }

        // input and update
        // TODO: Am I wasting a LOT of cycles here?
        c = wgetch(win);
        //c = getch();
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0], player[1] + 1)) {
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
                }
                break;
            // no default actions to be taken
        }

        // If you reach the end, start over in a new maze
        if (player[0] == finish[0] && player[1] == finish[1]) {
            success_splash(count + 2);
            wclear(win);
            box(win, 0, 0);
            wrefresh(win);
            get_new_dims(nrows, ncols, count);

            // generate a new maze
            backtracking_maze_gen(maze, MAX_MAZE_SIZE, nrows, ncols);
            gen_entrances_opposites(maze, MAX_MAZE_SIZE, player, finish, nrows, ncols);

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
    bool maze[MAX_MAZE_SIZE * MAX_MAZE_SIZE /2];
    bool visited[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
    int nrows(19);
    int ncols(31);
    int player[2] = {1, 1};
    int finish[2] = {1, 1};
    int count(0);
    int c;
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
    box(win, 0, 0);
    last_win_y = win_y;
    last_win_x = win_x;

    // generate a new maze
    std::fill_n(visited, MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2, false);
    backtracking_maze_gen(maze, MAX_MAZE_SIZE, nrows, ncols);
    gen_entrances_opposites(maze, MAX_MAZE_SIZE, player, finish, nrows, ncols);

    while (true) {
        visited[player[0] * MAX_MAZE_SIZE + player[1]] = true;
        visited[finish[0] * MAX_MAZE_SIZE + finish[1]] = true;
        maze_print_medium(win, maze, visited, MAX_MAZE_SIZE, nrows, ncols, player, finish);

        // input and update
        c = getch();
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, MAX_MAZE_SIZE, nrows, ncols, player[0], player[1] + 1)) {
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
                }
                break;
            // no default actions to be taken
        }

        // If you reach the end, start over in a new maze
        if (player[0] == finish[0] && player[1] == finish[1]) {
            success_splash(count + 2);
            wclear(win);
            box(win, 0, 0);
            wrefresh(win);
            get_new_dims(nrows, ncols, count);

            // generate a new maze
            std::fill_n(visited, MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2, false);
            backtracking_maze_gen(maze, MAX_MAZE_SIZE, nrows, ncols);
            gen_entrances_opposites(maze, MAX_MAZE_SIZE, player, finish, nrows, ncols);

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


// TODO: Put in a box, and handle longer sentences (split on spaces?).
/**
 *  A quick splash screen to congratulate the player on finishing the maze.
 *  NOTE: This is partially just a place-holder for a better splash screen.
 */
void success_splash(int count) {
    clear();

    mvprintw(1, 1, (std::string(splash_exclaim[rand() % n_splash_exclaim]) +
                    std::string(splash_success[rand() % n_splash_success])).c_str());
    mvprintw(3, 1, splash_story[rand() % n_splash_story]);
    mvprintw(7, 1, (std::string("You are ") +
                    std::to_string(count) +
                    std::string(" levels under Erebor.")).c_str());

    refresh();
    getch();
}

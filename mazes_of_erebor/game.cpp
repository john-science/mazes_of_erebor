
#include <random>
#include <ncurses.h>
#include <string.h>
#include "maze.h"
#include "menu.h"

// forward declarations
void success_splash(int count);
void get_new_dims(int& nrows, int& ncols, int count);
game_state game_ui(WINDOW *menu_win, game_state state);
game_state game_ui_medium(WINDOW *menu_win);

// constants for splash screen
const char* splash_exclaim[] = {"", "Success! ", "Finally! ", "Whew! "};
const int n_splash_exclaim = sizeof(splash_exclaim) / sizeof(char *);
const char* splash_success[] = {"You found a way out!",
                                "You are through the maze!",
                                "You did it!",
                                "You found your way through the maze!",
                                "You found the end of the laybrinth!",};
const int n_splash_success = sizeof(splash_success) / sizeof(char *);
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
game_state game_ui(WINDOW *menu_win, game_state state)
{
    const int MAX_SIZE(71);
    bool maze[MAX_SIZE * MAX_SIZE];
    int nrows(19);
    int ncols(31);
    int player[2] = {1, 1};
    int finish[2] = {1, 1};
    int count(0);
    int c;

    // generate a new maze
    backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
    gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);

    while (true) {
        if (state == game_easy) {
            maze_print_easy(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);
        } else if (state == game_hard) {
            maze_print_hard(menu_win, maze, MAX_SIZE, nrows, ncols, player, finish);
        }

        // input and update
        c = getch();
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] + 1)) {
                    player[1] += 1;
                }
                break;
            case 113:  // q
                return menu_main;
            default:
                break;
        }

        // If you reach the end, start over in a new maze
        if (player[0] == finish[0] && player[1] == finish[1]) {
            success_splash(count + 2);
            get_new_dims(nrows, ncols, count);

            // generate a new maze
            backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
            gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);

            count += 1;
        }
    }

    clear();
}


/**
 *   The basic maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
game_state game_ui_medium(WINDOW *menu_win)
{
    const int MAX_SIZE(71);
    bool maze[MAX_SIZE * MAX_SIZE];
    bool visited[MAX_SIZE * MAX_SIZE];
    int nrows(19);
    int ncols(31);
    int player[2] = {1, 1};
    int finish[2] = {1, 1};
    int count(0);
    int c;

    // generate a new maze
    std::fill_n(visited, MAX_SIZE * MAX_SIZE, false);
    backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
    gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);

    while (true) {
        visited[player[0] * MAX_SIZE + player[1]] = true;
        maze_print_medium(menu_win, maze, visited, MAX_SIZE, nrows, ncols, player, finish);

        // input and update
        c = getch();
        switch (c) {
            case KEY_UP:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] - 1, player[1])) {
                    player[0] -= 1;
                }
                break;
            case KEY_DOWN:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0] + 1, player[1])) {
                    player[0] += 1;
                }
                break;
            case KEY_LEFT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] - 1)) {
                    player[1] -= 1;
                }
                break;
            case KEY_RIGHT:
                if (maze_valid_move(maze, MAX_SIZE, nrows, ncols, player[0], player[1] + 1)) {
                    player[1] += 1;
                }
                break;
            case 113:  // q
                return menu_main;
            default:
                break;
        }

        // If you reach the end, start over in a new maze
        if (player[0] == finish[0] && player[1] == finish[1]) {
            success_splash(count + 2);
            get_new_dims(nrows, ncols, count);

            // generate a new maze
            std::fill_n(visited, MAX_SIZE * MAX_SIZE, false);
            backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
            gen_entrances_opposites(maze, MAX_SIZE, player, finish, nrows, ncols);

            count += 1;
        }
    }

    clear();
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

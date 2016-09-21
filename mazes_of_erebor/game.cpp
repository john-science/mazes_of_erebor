
#include <random>
#include <ncurses.h>
#include <string.h>
#include <vector>
#include "maze.h"
#include "menu.h"
#include "windows.h"

using namespace std;

// forward declarations
void get_new_dims(int& nrows, int& ncols, int count);
game_state game_ui(WINDOW *menu_win, game_state state);
game_state game_ui_medium(WINDOW *menu_win);
void success_splash(WINDOW *win, int count);
void content_screen(WINDOW *win, string txt);

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
const char* splash_story[] = {"You delve deeper.",
                              "You kick over a dusty old pile of Orcish remains that block the staircase.",
                              "You take a short rest before taking the staircase down.",
                              "At the end of the maze you find a staircase leading down.",
                              "You find a staircase leading down and follow it.",
                              "Deeper and deeper into the Halls of the Mountain King...",
                              "Your torch flickers in a draft as you head down the stairs.",
                              "You hear the echoes of war drums far off in the distance.",
                              "You find a curving ramp leading further down into the mountain.",
                              "You find a narrow staircase leading down into the mountain.",
                              "How deep under the mountain does these tunnels go?",
                              "Above the stone doorway you find an engraved scene of a human archer killing a dragon.",
                              "Engraved along the walls of the spiral staircase are scenes of a dwarf being buried with a glowing gem."};
const int n_splash_story = sizeof(splash_story) / sizeof(char *);
const char* intro = "You are a young dwarf in the late fourth age of this world.\n\n"
                    "The number of dwarves in the world has dwindles and foul creatures have "
                    "taken over your home Erebor, the Lonely Mountain. You are too late to "
                    "save your people, but deep under the mountain, in the labyrnths and "
                    "catacombs a weapon is buried that is said can destroy the "
                    "Lonely Mountain.\n\nThe last thing you want is to destroy your homeland, "
                    "but the hoards in the mountain grow strong and you can't let them "
                    "survive to attack Middle Earth.\n\nSo you head into the catacombs with "
                    "no thought over ever getting out.";  // TODO: "weapon is" overruns a line


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
    content_screen(win, intro);
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
    content_screen(win, intro);
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


/**
 *    Format potentially long text for print-out.
 */
vector<string> format_text(const string txt, unsigned int num_cols) {
    vector<string> lines;
    unsigned int i(0);
    unsigned int last_space(0);
    unsigned int last_end(0);

    // if the line is short, skip all this work
    if (txt.length() < num_cols) {
        lines.push_back(txt.substr(0));
        return lines;
    }

    // loop through each character
    while (i < txt.length()) {
        // note the spaces, for clean line breaks
        if (txt.at(i) == ' '){
            last_space = i;
        }

        // break on EOL and when you are past the column count
        if (txt.at(i) == '\n') {
            lines.push_back(txt.substr(last_end, i - last_end));
            last_end = i + 1;
        } else if ((i - last_end) == num_cols) {
            if (last_end >= last_space) {
                lines.push_back(txt.substr(last_end, num_cols));
                last_end += num_cols;
            } else {
                lines.push_back(txt.substr(last_end, last_space - last_end));
                last_end = last_space + 1;
            }
        }
        i += 1;
    }

    // add last part of string
    lines.push_back(txt.substr(last_end));
    return lines;
}


/**
 *    Splash screen, after you finish a maze
 */
void success_splash(WINDOW *win, int count) {
    string txt(string(splash_exclaim[rand() % n_splash_exclaim]) +
               string(splash_success[rand() % n_splash_success]) + "\n\n" +
               splash_story[rand() % n_splash_story] + "\n\n\n" +
               string("You are now ") + to_string(count) + string(" levels under Erebor."));

    content_screen(win, txt);
}


/**
 *    Default function to display text content
 */
void content_screen(WINDOW *win, string txt) {
    int win_y(15);
    int win_x(15);
    mvwin(win, 0, 0);
    getmaxyx(stdscr, win_y, win_x);
    wresize(win, win_y, win_x);
    wclear(win);
    box(win, 0, 0);

    if (win_y < 6 || win_x < 12) {return;}

    unsigned int num_rows(win_y - 2);
    int num_cols(win_x - 4);

    vector<string> lines(format_text(txt, num_cols));
    unsigned int row(1);

    for (int i=0; i < (int)lines.size(); ++i) {
        mvprintw(row, 2, lines[i].c_str());  // TODO: Should probably be a vector of const char*
        row += 1;
        if (row == num_rows) {
            row = 0;
            wrefresh(win);
            getch();
            wclear(win);
            box(win, 0, 0);
        }
    }

    wrefresh(win);
    getch();
}

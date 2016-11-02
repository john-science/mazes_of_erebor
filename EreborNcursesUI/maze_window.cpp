
#include <ncurses.h>
#include <functional>
#include "data.h"
#include "game.h"
#include "windows.h"
#include "splash.h"

using namespace std;

// forward declarations
void maze_print_easy(WINDOW*, const GameData);
void maze_print_medium(WINDOW*, const GameData);
void maze_print_hard(WINDOW*, const GameData);
void get_printing_dimensions(WINDOW* win, const GameData d, int& min_x, int& max_x, int& min_y, int& max_y, int& r_off, int& c_off);


/**
 *   The game maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
menu_state game_loop_maze(WINDOW *win, GameData *d, menu_state state) {
    int win_y(15);
    int win_x(15);
    int last_win_y, last_win_x;

    // init window at current resolution
    if (d->maze.level == -1) {
        intro_splash(win, d->player);
    }
    init_maze_window(win);
    getmaxyx(stdscr, win_y, win_x);
    last_win_y = win_y;
    last_win_x = win_x;

    // select the appropriate print function
    function<void(WINDOW*, const GameData)> maze_print = \
        state == game_easy ? maze_print_easy : (state == game_medium ? maze_print_medium : maze_print_hard);

    // generate a new maze, if necessary
    if (d->maze.level == -1) {
        maze_loop_init(d, state);
    }

    int c;
    int level(d->maze.level);
    bool needs_update(true);

    // GAME LOOP
    while (true) {
        // RENDER
        if (level != d->maze.level) {
            level = d->maze.level;
            if (!d->reached_end()) {
                success_splash(win, level + 1);
            }
        }
        if (needs_update) {
            if (d->reached_end()) {
                return game_win;
            } else {
                maze_print(win, *d);
                needs_update = false;
            }
        }

        // INPUT
        c = wgetch(win);
        switch (c) {
            case 113:  // q
                return menu_cont;
            case KEY_RESIZE:
                getmaxyx(stdscr, win_y, win_x);
                if (last_win_x != win_x || last_win_y != win_y) {
                    last_win_y = win_y;
                    last_win_x = win_x;
                    full_box_resize(win, win_y, win_x);
                }
                needs_update = true;
                c = -999;
                break;
            // no default actions to be taken
        }

        // UPDATE
        maze_loop_update(c, &needs_update, d);
    }
}


/**
 * Displaying end game "win" text.
 */
menu_state game_win_screen(WINDOW *win, const GameData *d) {
    win_splash(win);
    wgetch(win);
    return quit;
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints from a God's Eye perspective,
 *   where the entire maze is visible.
 */
void maze_print_easy(WINDOW *win, const GameData d)
{
    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, d, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    for (int r=min_y; r < max_y; r++) {
        for (int c=min_x; c < max_x; c++) {
            if (d.player.loc[0] == r && d.player.loc[1] == c) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, "@");  // player
                wattroff(win, COLOR_PAIR(6));
            } else if (d.maze.finish[0] == r && d.maze.finish[1] == c) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, "X");  // finish
                wattroff(win, COLOR_PAIR(6));
            } else if (!d.maze.get(r, c)) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, " ");  // hallway
                wattroff(win, COLOR_PAIR(6));
            } else {
                wattron(win, COLOR_PAIR(3));
                mvwprintw(win, r + r_off, c + c_off, "#");  // wall
                wattroff(win, COLOR_PAIR(3));  // necessary to fix game window box color
            }
        }
    }

    refresh();
    wrefresh(win);
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints the maze from a static top-down position,
 *   but only displays the cells that are line-of-sight visible
 *   to a player carrying an infinitely-bright light source.
 */
void maze_print_medium(WINDOW *win, const GameData d)
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    const int finish_posi(3);
    int r(0);
    int c(0);
    int grid[d.maze.nrows * d.maze.ncols];
    fill_n(grid, d.maze.nrows * d.maze.ncols, 0);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = d.player.loc[0];
    c = d.player.loc[1];
    while (c < (d.maze.ncols - 1) && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (r < (d.maze.nrows - 1)) {
            if (!d.maze.get(r + 1, c)) {grid[(r + 1) * d.maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!d.maze.get(r - 1, c)) {grid[(r - 1) * d.maze.ncols + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = d.player.loc[1];
    while (c > 0 && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (r < (d.maze.nrows - 1)) {
            if (!d.maze.get(r + 1, c)) {grid[(r + 1) * d.maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!d.maze.get(r - 1, c)) {grid[(r - 1) * d.maze.ncols + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = d.player.loc[0];
    c = d.player.loc[1];
    while (r < (d.maze.nrows - 1) && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (c < (d.maze.ncols - 1)) {
            if (!d.maze.get(r, c + 1)) {grid[r * d.maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!d.maze.get(r, c - 1)) {grid[r * d.maze.ncols + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = d.player.loc[0];
    while (r > 0 && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (c < (d.maze.ncols - 1)) {
            if (!d.maze.get(r, c + 1)) {grid[r * d.maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!d.maze.get(r, c - 1)) {grid[r * d.maze.ncols + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[d.player.loc[0] * d.maze.ncols + d.player.loc[1]] = player_posi;
    grid[d.maze.finish[0] * d.maze.ncols + d.maze.finish[1]] = finish_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, d, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * d.maze.ncols + c];
            if (cell == 0) {
                if (d.player.visited[r * d.maze.ncols + c]) {
                    wattron(win, COLOR_PAIR(5));
                    mvwprintw(win, r + r_off, c + c_off, " ");  // visited
                } else {
                    wattron(win, COLOR_PAIR(1));
                    mvwprintw(win, r + r_off, c + c_off, " ");  // empty
                    wattroff(win, COLOR_PAIR(1));  // necessary to fix game window box color
                }
            } else if (cell == open_hall) {
                wattron(win, COLOR_PAIR(3));
                mvwprintw(win, r + r_off, c + c_off, "#");      // hallway
            } else if (cell == player_posi) {
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, r + r_off, c + c_off, "@");      // player
            } else if (cell == finish_posi) {
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, r + r_off, c + c_off, "X");      // finish
            }
        }
    }

    refresh();
    wrefresh(win);
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints the maze from a static top-down position,
 *   but only displays the cells that are line-of-sight visible
 *   to a player carrying an infinitely-bright light source.
 */
void maze_print_hard(WINDOW *win, const GameData d)
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    int r(0);
    int c(0);
    int grid[d.maze.nrows * d.maze.ncols];
    fill_n(grid, d.maze.nrows * d.maze.ncols, 0);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = d.player.loc[0];
    c = d.player.loc[1];
    while (c < (d.maze.ncols - 1) && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (r < (d.maze.nrows - 1)) {
            if (!d.maze.get(r + 1, c)) {grid[(r + 1) * d.maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!d.maze.get(r - 1, c)) {grid[(r - 1) * d.maze.ncols + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = d.player.loc[1];
    while (c > 0 && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (r < (d.maze.nrows - 1)) {
            if (!d.maze.get(r + 1, c)) {grid[(r + 1) * d.maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!d.maze.get(r - 1, c)) {grid[(r - 1) * d.maze.ncols + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = d.player.loc[0];
    c = d.player.loc[1];
    while (r < (d.maze.nrows - 1) && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (c < (d.maze.ncols - 1)) {
            if (!d.maze.get(r, c + 1)) {grid[r * d.maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!d.maze.get(r, c - 1)) {grid[r * d.maze.ncols + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = d.player.loc[0];
    while (r > 0 && !d.maze.get(r, c)) {
        grid[r * d.maze.ncols + c] = open_hall;
        if (c < (d.maze.ncols - 1)) {
            if (!d.maze.get(r, c + 1)) {grid[r * d.maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!d.maze.get(r, c - 1)) {grid[r * d.maze.ncols + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[d.player.loc[0] * d.maze.ncols + d.player.loc[1]] = player_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, d, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    int diff;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * d.maze.ncols + c];
            if (cell == 0) {
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, r + r_off, c + c_off, " ");  // wall
                wattroff(win, COLOR_PAIR(1));  // necessary to fix game window box color
            } else if (cell == open_hall) {
                if (r == d.maze.finish[0] && c == d.maze.finish[1]) {
                    wattron(win, COLOR_PAIR(2));
                    mvwprintw(win, r + r_off, c + c_off, "X");  // finish
                } else {
                    // different colors depending on how far away the torch light reaches
                    diff = abs(d.player.loc[0] - r) + abs(d.player.loc[1] - c);
                    if (diff < 2){
                        wattron(win, COLOR_PAIR(3));
                    } else if (diff < 4){
                        wattron(win, COLOR_PAIR(4));
                    } else if (diff < 6){
                        wattron(win, COLOR_PAIR(5));
                    } else {
                        wattron(win, COLOR_PAIR(1));
                    }
                    mvwprintw(win, r + r_off, c + c_off, "#");  // hallway
                }
            } else if (cell == player_posi) {
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, r + r_off, c + c_off, "@");  // player
            }
        }
    }

    refresh();
    wrefresh(win);
}


/**
 *  Get the limits of the maze printing region,
 *  and check for the case where the maze is bigger than the screen.
 */
void get_printing_dimensions(WINDOW* win, const GameData d, int& min_x, int& max_x, int& min_y, int& max_y, int& r_off, int& c_off) {
    // get window dimensions
    int win_y, win_x;
    getmaxyx(stdscr, win_y, win_x);

    // coordinates of corner of maze, on the screen
    c_off = 1 + ((win_x - d.maze.ncols - 2) / 2);
    r_off = 1 + ((win_y - d.maze.nrows - 2) / 2);

    // region of maze that can be printed
    min_y = 0;
    min_x = 0;
    max_y = d.maze.nrows;
    max_x = d.maze.ncols;

    // if window is too small, limit the printed region of the maze
    if (max_y > (win_y - 2)) {
        min_y = d.player.loc[0] - (win_y - 2) / 2;
        max_y = win_y < MIN_WINDOW_SIZE ? 0 : d.player.loc[0] + (win_y - 2) / 2;
        r_off = 1 - min_y;
        if (min_y < 0) {min_y = 0;}
        if (max_y > d.maze.nrows) {max_y = d.maze.nrows;}
    }
    if (max_x > (win_x - 2)) {
        min_x = d.player.loc[1] - (win_x - 2) / 2;
        max_x = win_x < MIN_WINDOW_SIZE ? 0 : d.player.loc[1] + (win_x - 2) / 2;
        c_off = 1 - min_x;
        if (min_x < 0) {min_x = 0;}
        if (max_x > d.maze.ncols) {max_x = d.maze.ncols;}
    }
}

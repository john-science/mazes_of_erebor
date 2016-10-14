
#include <ncurses.h>
#include <functional>
#include "data.h"
#include "game.h"
#include "windows.h"
#include "splash.h"

using namespace std;

// forward declarations
void maze_print_easy(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_medium(WINDOW *win, const maze_data maze, const player_data player);
void maze_print_hard(WINDOW *win, const maze_data maze, const player_data player);
void get_printing_dimensions(WINDOW* win, const maze_data maze, const player_data p, \
                             int& min_x, int& max_x, int& min_y, int& max_y, int& r_off, int& c_off);


/**
 *   The game maze GUI.
 *
 *   Use arrow keys to navigate the maze or type "q" to quit.
 */
menu_state game_loop_maze(WINDOW *win, game_data *d, menu_state state) {
    maze_data *maze = &d->maze;
    player_data *player = &d->player;
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

    // select the appropriate print function
    function<void(WINDOW*, const maze_data, const player_data)> maze_print = \
        state == game_easy ? maze_print_easy : (state == game_medium ? maze_print_medium : maze_print_hard);

    // generate a new maze, if necessary
    if (maze->level == -1) {
        maze_loop_init(maze, player, state);
    }

    int c;
    int level(0);
    bool needs_update(true);

    // GAME LOOP
    while (true) {
        // RENDER
        if (level != maze->level) {
            level = maze->level;
            success_splash(win, level + 2);
        }
        if (needs_update) {
            maze_print(win, *maze, *player);
            needs_update = false;
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
        maze_loop_update(c, &needs_update, maze, player);
    }
}



// TODO: If the window is larger than the maze, we only need to redraw the player, not the maze.
/**
 *   Print a maze, including player/finish positions.
 *   This prints from a God's Eye perspective,
 *   where the entire maze is visible.
 */
void maze_print_easy(WINDOW *win, const maze_data maze, const player_data p)
{
    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, maze, p, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    for (int r=min_y; r < max_y; r++) {
        for (int c=min_x; c < max_x; c++) {
            if (p.loc[0] == r && p.loc[1] == c) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, "@");  // player
                wattroff(win, COLOR_PAIR(6));
            } else if (maze.finish[0] == r && maze.finish[1] == c) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, "X");  // finish
                wattroff(win, COLOR_PAIR(6));
            } else if (!maze.grid[c + r * maze.ncols]) {
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
void maze_print_medium(WINDOW *win, const maze_data maze, const player_data p)
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    const int finish_posi(3);
    int r(0);
    int c(0);
    int grid[maze.nrows * maze.ncols];
    fill_n(grid, maze.nrows * maze.ncols, 0);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = p.loc[0];
    c = p.loc[1];
    while (c < (maze.ncols - 1) && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.ncols + c]) {grid[(r + 1) * maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.ncols + c]) {grid[(r - 1) * maze.ncols + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = p.loc[1];
    while (c > 0 && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.ncols + c]) {grid[(r + 1) * maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.ncols + c]) {grid[(r - 1) * maze.ncols + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = p.loc[0];
    c = p.loc[1];
    while (r < (maze.nrows - 1) && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.ncols + c + 1]) {grid[r * maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.ncols + c - 1]) {grid[r * maze.ncols + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = p.loc[0];
    while (r > 0 && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.ncols + c + 1]) {grid[r * maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.ncols + c - 1]) {grid[r * maze.ncols + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[p.loc[0] * maze.ncols + p.loc[1]] = player_posi;
    grid[maze.finish[0] * maze.ncols + maze.finish[1]] = finish_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, maze, p, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * maze.ncols + c];
            if (cell == 0) {
                if (p.visited[r * maze.ncols + c]) {
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
void maze_print_hard(WINDOW *win, const maze_data maze, const player_data p)
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    int r(0);
    int c(0);
    int grid[maze.nrows * maze.ncols];
    fill_n(grid, maze.nrows * maze.ncols, 0);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = p.loc[0];
    c = p.loc[1];
    while (c < (maze.ncols - 1) && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.ncols + c]) {grid[(r + 1) * maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.ncols + c]) {grid[(r - 1) * maze.ncols + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = p.loc[1];
    while (c > 0 && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.ncols + c]) {grid[(r + 1) * maze.ncols + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.ncols + c]) {grid[(r - 1) * maze.ncols + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = p.loc[0];
    c = p.loc[1];
    while (r < (maze.nrows - 1) && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.ncols + c + 1]) {grid[r * maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.ncols + c - 1]) {grid[r * maze.ncols + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = p.loc[0];
    while (r > 0 && !maze.grid[r * maze.ncols + c]) {
        grid[r * maze.ncols + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.ncols + c + 1]) {grid[r * maze.ncols + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.ncols + c - 1]) {grid[r * maze.ncols + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[p.loc[0] * maze.ncols + p.loc[1]] = player_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, maze, p, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    int diff;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * maze.ncols + c];
            if (cell == 0) {
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, r + r_off, c + c_off, " ");  // wall
                wattroff(win, COLOR_PAIR(1));  // necessary to fix game window box color
            } else if (cell == open_hall) {
                if (r == maze.finish[0] && c == maze.finish[1]) {
                    wattron(win, COLOR_PAIR(2));
                    mvwprintw(win, r + r_off, c + c_off, "X");  // finish
                } else {
                    // different colors depending on how far away the torch light reaches
                    diff = abs(p.loc[0] - r) + abs(p.loc[1] - c);
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
void get_printing_dimensions(WINDOW* win, const maze_data maze, const player_data p, \
                             int& min_x, int& max_x, int& min_y, int& max_y, int& r_off, int& c_off) {
    // get window dimensions
    int win_y, win_x;
    getmaxyx(stdscr, win_y, win_x);

    // coordinates of corner of maze, on the screen
    c_off = 1 + ((win_x - maze.ncols - 2) / 2);
    r_off = 1 + ((win_y - maze.nrows - 2) / 2);

    // region of maze that can be printed
    min_y = 0;
    min_x = 0;
    max_y = maze.nrows;
    max_x = maze.ncols;

    // if window is too small, limit the printed region of the maze
    if (max_y > (win_y - 2)) {
        min_y = p.loc[0] - (win_y - 2) / 2;
        max_y = win_y < MIN_WINDOW_SIZE ? 0 : p.loc[0] + (win_y - 2) / 2;
        r_off = 1 - min_y;
        if (min_y < 0) {min_y = 0;}
        if (max_y > maze.nrows) {max_y = maze.nrows;}
    }
    if (max_x > (win_x - 2)) {
        min_x = p.loc[1] - (win_x - 2) / 2;
        max_x = win_x < MIN_WINDOW_SIZE ? 0 : p.loc[1] + (win_x - 2) / 2;
        c_off = 1 - min_x;
        if (min_x < 0) {min_x = 0;}
        if (max_x > maze.ncols) {max_x = maze.ncols;}
    }
}

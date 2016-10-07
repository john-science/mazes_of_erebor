
#include <iostream>
#include <ncurses.h>
#include <stack>
#include <algorithm>
#include <random>
#include "data.h"
#include "windows.h"

using namespace std;

// forward declarations
static void get_new_dims(int& nrows, int& ncols, int level);
void backtracking_maze_gen(maze_data *maze);
void gen_entrances_opposites(maze_data *maze);
void get_printing_dimensions(WINDOW* win, const maze_data maze, const player_data p, \
                             int& min_x, int& max_x, int& min_y, int& max_y, int& r_off, int& c_off);


/**
    Get an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
bool maze_get(const maze_data maze, int row, int col)
{
    return maze.grid[col + row * maze.max_size];
}


/**
    Set an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
void maze_set(maze_data *maze, int row, int col, bool value)
{
    maze->grid[col + row * maze->max_size] = value;
}


/**
    Find a random, un-opened neighbor of a maze cell.
*/
int* find_neighbor(const maze_data maze, int row, int col, int result[])
{
    int order[4] = {0, 1, 2, 3};
    result[0] = -999;
    result[1] = -999;
    random_shuffle(&order[0], &order[4]);

    for (int i = 0; i < 4; i++){
        if (order[i] == 0){
            if (row > 1 && maze_get(maze, row - 2, col)){
                result[0] = row - 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 1){
            if (row < maze.nrows - 2 && maze_get(maze, row + 2, col)) {
                result[0] = row + 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 2) {
            if (col > 1 && maze_get(maze, row, col - 2)) {
                result[0] = row;
                result[1] = col - 2;
                return result;
            }
        } else {
            if (col < maze.ncols - 2 && maze_get(maze, row, col + 2)) {
                result[0] = row;
                result[1] = col + 2;
                return result;
            }
        }
    }

    return result;
}


/**
    Generate a maze using the backtracking algorithm.
*/
void backtracking_maze_gen(maze_data *maze)
{
    int row, col, r, c;
    int neighbor[2] = {-999, -999};
    stack <int> track;

    // ensure maze is correctly initialized
    fill_n(maze->grid, maze->max_size * maze->max_size / 2, true);

    // pick a random starting point
    row = 1 + 2 * (rand() % ((maze->nrows - 1) / 2));
    col = 1 + 2 * (rand() % ((maze->ncols - 1) / 2));
    track.push(row);
    track.push(col);
    maze_set(maze, row, col, false);

    while (track.size() > 0) {
        // choose a grid cell from the track
        col = track.top();
        track.pop();
        row = track.top();
        track.pop();

        // get open neighbors in random order
        find_neighbor(*maze, row, col, neighbor);

        if (neighbor[0] > 0) {
            r = neighbor[0];
            c = neighbor[1];

            // dig a hallway to it & add it to frontier
            maze_set(maze, r, c, false);
            maze_set(maze, (row + r) / 2, (col + c) / 2, false);
            track.push(row);
            track.push(col);
            track.push(r);
            track.push(c);
        }
    }
}


/**
    Generate random maze start/finish positions.
*/
void gen_entrances_opposites(maze_data *maze)
{
    int wall;
    wall = rand() % 4;

    if (wall / 2 == 0) {
        // East-West walls
        maze->start[0] = (rand() % ((maze->nrows - 1) / 2)) * 2 + 1;
        maze->finish[0] = (rand() % ((maze->nrows - 1) / 2)) * 2 + 1;
        if (wall % 2 == 0) {
            // Start on West wall
            maze->start[1] = 0;
            maze->finish[1] = maze->ncols - 1;
        } else {
            // Start on East Wall
            maze->start[1] = maze->ncols - 1;
            maze->finish[1] = 0;
        }
    } else {
        // North-South walls
        maze->start[1] = (rand() % ((maze->ncols - 1) / 2)) * 2 + 1;
        maze->finish[1] = (rand() % ((maze->ncols - 1) / 2)) * 2 + 1;
        if (wall % 2 == 0) {
            // Start on North wall
            maze->start[0] = 0;
            maze->finish[0] = maze->nrows - 1;
        } else {
            // Start on South Wall
            maze->start[0] = maze->nrows - 1;
            maze->finish[0] = 0;
        }
    }

    maze->grid[maze->start[0] * maze->max_size + maze->start[1]] = false;
    maze->grid[maze->finish[0] * maze->max_size + maze->finish[1]] = false;
}


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
            } else if (!maze.grid[c + r * maze.max_size]) {
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
    int grid[maze.nrows * maze.max_size + maze.ncols];  // TODO: Dims all wrong?
    fill_n(grid, maze.nrows * maze.max_size + maze.ncols, 0);  // TODO: Dims all wrong?

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = p.loc[0];
    c = p.loc[1];
    while (c < (maze.ncols - 1) && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.max_size + c]) {grid[(r + 1) * maze.max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.max_size + c]) {grid[(r - 1) * maze.max_size + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = p.loc[1];
    while (c > 0 && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.max_size + c]) {grid[(r + 1) * maze.max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.max_size + c]) {grid[(r - 1) * maze.max_size + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = p.loc[0];
    c = p.loc[1];
    while (r < (maze.nrows - 1) && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.max_size + c + 1]) {grid[r * maze.max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.max_size + c - 1]) {grid[r * maze.max_size + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = p.loc[0];
    while (r > 0 && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.max_size + c + 1]) {grid[r * maze.max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.max_size + c - 1]) {grid[r * maze.max_size + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[p.loc[0] * maze.max_size + p.loc[1]] = player_posi;
    grid[maze.finish[0] * maze.max_size + maze.finish[1]] = finish_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, maze, p, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * maze.max_size + c];
            if (cell == 0) {
                if (p.visited[r * maze.max_size + c]) {
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
    int grid[maze.nrows * maze.max_size + maze.ncols];
    fill_n(grid, maze.nrows * maze.max_size + maze.ncols, 0);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = p.loc[0];
    c = p.loc[1];
    while (c < (maze.ncols - 1) && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.max_size + c]) {grid[(r + 1) * maze.max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.max_size + c]) {grid[(r - 1) * maze.max_size + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = p.loc[1];
    while (c > 0 && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (r < (maze.nrows - 1)) {
            if (!maze.grid[(r + 1) * maze.max_size + c]) {grid[(r + 1) * maze.max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze.grid[(r - 1) * maze.max_size + c]) {grid[(r - 1) * maze.max_size + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = p.loc[0];
    c = p.loc[1];
    while (r < (maze.nrows - 1) && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.max_size + c + 1]) {grid[r * maze.max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.max_size + c - 1]) {grid[r * maze.max_size + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = p.loc[0];
    while (r > 0 && !maze.grid[r * maze.max_size + c]) {
        grid[r * maze.max_size + c] = open_hall;
        if (c < (maze.ncols - 1)) {
            if (!maze.grid[r * maze.max_size + c + 1]) {grid[r * maze.max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze.grid[r * maze.max_size + c - 1]) {grid[r * maze.max_size + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[p.loc[0] * maze.max_size + p.loc[1]] = player_posi;

    int min_x, max_x, min_y, max_y, c_off, r_off;
    get_printing_dimensions(win, maze, p, min_x, max_x, min_y, max_y, r_off, c_off);

    clear();
    wclear(win);

    // do the actual (colored) printing
    int cell;
    int diff;
    for (r=min_y; r < max_y; r++) {
        for (c=min_x; c < max_x; c++) {
            cell = grid[r * maze.max_size + c];
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


/**
 *  Determine if a particular grid cell is a valid move.
 */
bool maze_valid_move(const maze_data maze, int r, int c) {
    if (r < 0 || c < 0) {
        return false;
    } else if (r >= maze.nrows) {
        return false;
    } else if (c >= maze.ncols) {
        return false;
    } else {
        return !maze.grid[r * maze.max_size + c];
    }
}


/**
 *   Randomly generate maze dimensions.
 */
static void get_new_dims(int& nrows, int& ncols, int level) {
    level %= 20;

    const int bottom_y = 15;
    nrows = bottom_y + level / 2 + (rand() % (int)(level / 2 + 1));
    if (nrows % 2 == 0) { nrows += 1; }

    const int bottom_x = 31;
    ncols = bottom_x + level + (int)(rand() % (level + 1));
    if (ncols % 2 == 0) { ncols += 1; }
}


/**
 *   Pull everything together and generate a new maze.
 */
void gen_new_maze(maze_data *maze) {
    get_new_dims(maze->nrows, maze->ncols, maze->level);
    backtracking_maze_gen(maze);
    gen_entrances_opposites(maze);
}

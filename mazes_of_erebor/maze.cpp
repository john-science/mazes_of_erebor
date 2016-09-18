
#include <iostream>
#include <ncurses.h>
#include <stack>
#include <algorithm>
#include <random>
#include "windows.h"

using namespace std;


/**
    Get an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
bool maze_get(bool maze[], int max_size, int row, int col)
{
    return maze[col + row * max_size];
}


/**
    Set an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
void maze_set(bool maze[], int max_size, int row, int col, bool value)
{
    maze[col + row * max_size] = value;
}


/**
    Find a random, un-opened neighbor of a maze cell.
*/
int* find_neighbor(bool maze[], int nrows, int ncols, int row, int col, int max_size, int result[])
{
    int order[4] = {0, 1, 2, 3};
    result[0] = -999;
    result[1] = -999;
    random_shuffle(&order[0], &order[4]);

    for (int i = 0; i < 4; i++){
        if (order[i] == 0){
            if (row > 1 && maze_get(maze, max_size, row - 2, col)){
                result[0] = row - 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 1){
            if (row < nrows - 2 && maze_get(maze, max_size, row + 2, col)) {
                result[0] = row + 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 2) {
            if (col > 1 && maze_get(maze, max_size, row, col - 2)) {
                result[0] = row;
                result[1] = col - 2;
                return result;
            }
        } else {
            if (col < ncols - 2 && maze_get(maze, max_size, row, col + 2)) {
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
void backtracking_maze_gen(bool maze[], int max_size, int nrows, int ncols)
{
    int row, col, r, c;
    int neighbor[2] = {-999, -999};
    stack <int> track;

    // ensure maze is correctly initialized
    fill_n(maze, max_size * max_size / 2, true);

    // pick a random starting point
    row = 1 + 2 * (rand() % ((nrows - 1) / 2));
    col = 1 + 2 * (rand() % ((ncols - 1) / 2));
    track.push(row);
    track.push(col);
    maze_set(maze, max_size, row, col, false);

    while (track.size() > 0) {
        // choose a grid cell from the track
        col = track.top();
        track.pop();
        row = track.top();
        track.pop();

        // get open neighbors in random order
        find_neighbor(maze, nrows, ncols, row, col, max_size, neighbor);

        if (neighbor[0] > 0) {
            r = neighbor[0];
            c = neighbor[1];

            // dig a hallway to it & add it to frontier
            maze_set(maze, max_size, r, c, false);
            maze_set(maze, max_size, (row + r) / 2, (col + c) / 2, false);
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
void gen_entrances_opposites(bool maze[], int max_size, int start[], int finish[], int nrows, int ncols)
{
    int wall;
    wall = rand() % 4;

    if (wall / 2 == 0) {
        // East-West walls
        start[0] = (rand() % ((nrows - 1) / 2)) * 2 + 1;
        finish[0] = (rand() % ((nrows - 1) / 2)) * 2 + 1;
        if (wall % 2 == 0) {
            // Start on West wall
            start[1] = 0;
            finish[1] = ncols - 1;
        } else {
            // Start on East Wall
            start[1] = ncols - 1;
            finish[1] = 0;
        }
    } else {
        // North-South walls
        start[1] = (rand() % ((ncols - 1) / 2)) * 2 + 1;
        finish[1] = (rand() % ((ncols - 1) / 2)) * 2 + 1;
        if (wall % 2 == 0) {
            // Start on North wall
            start[0] = 0;
            finish[0] = nrows - 1;
        } else {
            // Start on South Wall
            start[0] = nrows - 1;
            finish[0] = 0;
        }
    }

    maze[start[0] * max_size + start[1]] = false;
    maze[finish[0] * max_size + finish[1]] = false;
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints from a God's Eye perspective,
 *   where the entire maze is visible.
 */
void maze_print_easy(WINDOW *win, bool maze[], int max_size, int nrows, int ncols, int player[], int finish[])
{
    int win_y, win_x;
    getmaxyx(stdscr, win_y, win_x);
    //init_maze_window(win, win_y, win_x);
    int c_off(1 + ((win_x - ncols - 2) / 2));
    int r_off(1 + ((win_y - nrows - 2) / 2));
    if (c_off < 1) { c_off = 1; }
    if (r_off < 1) { r_off = 1; }

    // TODO: Center this region around the player?
    // if window is too small, limit the printed region of the maze
    int max_y(nrows);
    int max_x(ncols);
    if (max_y > (win_y - 2)) { max_y = win_y < 5 ? 0 : win_y - 2; }
    if (max_x > (win_x - 2)) { max_x = win_x < 5 ? 0 : win_x - 2; }

    // open up hallways
    for (int r=0; r < max_y; r++) {
        for (int c=0; c < max_x; c++) {
            if (player[0] == r && player[1] == c) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, "@");  // player
            } else if (!maze[c + r * max_size]) {
                wattron(win, COLOR_PAIR(6));
                mvwprintw(win, r + r_off, c + c_off, " ");  // hallway
            } else {
                wattron(win, COLOR_PAIR(3));
                mvwprintw(win, r + r_off, c + c_off, "#");  // wall
                wattroff(win, COLOR_PAIR(3));  // necessary to fix game window box color
            }
        }
    }
    wrefresh(win);
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints the maze from a static top-down position,
 *   but only displays the cells that are line-of-sight visible
 *   to a player carrying an infinitely-bright light source.
 */
void maze_print_medium(WINDOW *win, bool maze[], const bool visited[], int max_size, int nrows, int ncols, int player[], int finish[])
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    int r(0);
    int c(0);
    int cell;
    int grid[nrows * max_size + ncols];  // TODO: Dims all wrong?
    fill_n(grid, nrows * max_size + ncols, 0);  // TODO: Dims all wrong?
    int win_y, win_x;
    getmaxyx(stdscr, win_y, win_x);
    //init_maze_window(win);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = player[0];
    c = player[1];
    while (c < (ncols - 1) && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (r < (nrows - 1)) {
            if (!maze[(r + 1) * max_size + c]) {grid[(r + 1) * max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze[(r - 1) * max_size + c]) {grid[(r - 1) * max_size + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = player[1];
    while (c > 0 && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (r < (nrows - 1)) {
            if (!maze[(r + 1) * max_size + c]) {grid[(r + 1) * max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze[(r - 1) * max_size + c]) {grid[(r - 1) * max_size + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = player[0];
    c = player[1];
    while (r < (nrows - 1) && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (c < (ncols - 1)) {
            if (!maze[r * max_size + c + 1]) {grid[r * max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze[r * max_size + c - 1]) {grid[r * max_size + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = player[0];
    while (r > 0 && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (c < (ncols - 1)) {
            if (!maze[r * max_size + c + 1]) {grid[r * max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze[r * max_size + c - 1]) {grid[r * max_size + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[player[0] * max_size + player[1]] = player_posi;

    // offsets for printing
    int c_off(1 + (win_x - 2 - ncols) / 2);
    int r_off(1 + (win_y - 1 - nrows) / 2);

    // open up hallways
    for (r=0; r < nrows; r++) {
        for (c=0; c < ncols; c++) {
            cell = grid[r * max_size + c];
            if (cell == 0) {
                if (visited[r * max_size + c]) {
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
            }
        }
    }
    wrefresh(win);
}


/**
 *   Print a maze, including player/finish positions.
 *   This prints the maze from a static top-down position,
 *   but only displays the cells that are line-of-sight visible
 *   to a player carrying an infinitely-bright light source.
 */
void maze_print_hard(WINDOW *win, bool maze[], int max_size, int nrows, int ncols, int player[], int finish[])
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    const int open_hall(1);
    const int player_posi(2);
    int r(0);
    int c(0);
    int cell;
    int grid[nrows * max_size + ncols];
    fill_n(grid, nrows * max_size + ncols, 0);
    int win_y, win_x;
    getmaxyx(stdscr, win_y, win_x);
    //init_maze_window(win);

    // start at the player and go in all 4 directions, looking for deadends
    // try going East
    r = player[0];
    c = player[1];
    while (c < (ncols - 1) && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (r < (nrows - 1)) {
            if (!maze[(r + 1) * max_size + c]) {grid[(r + 1) * max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze[(r - 1) * max_size + c]) {grid[(r - 1) * max_size + c] = open_hall;}
        }
        c += 1;
    }
    // try going West
    c = player[1];
    while (c > 0 && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (r < (nrows - 1)) {
            if (!maze[(r + 1) * max_size + c]) {grid[(r + 1) * max_size + c] = open_hall;}
        }
        if (r > 0) {
            if (!maze[(r - 1) * max_size + c]) {grid[(r - 1) * max_size + c] = open_hall;}
        }
        c -= 1;
    }
    // try going North
    r = player[0];
    c = player[1];
    while (r < (nrows - 1) && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (c < (ncols - 1)) {
            if (!maze[r * max_size + c + 1]) {grid[r * max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze[r * max_size + c - 1]) {grid[r * max_size + c - 1] = open_hall;}
        }
        r += 1;
    }
    // try going South
    r = player[0];
    while (r > 0 && !maze[r * max_size + c]) {
        grid[r * max_size + c] = open_hall;
        if (c < (ncols - 1)) {
            if (!maze[r * max_size + c + 1]) {grid[r * max_size + c + 1] = open_hall;}
        }
        if (c > 0) {
            if (!maze[r * max_size + c - 1]) {grid[r * max_size + c - 1] = open_hall;}
        }
        r -= 1;
    }
    grid[player[0] * max_size + player[1]] = player_posi;

    // offsets for printing
    int c_off(1 + (win_x - 2 - ncols) / 2);
    int r_off(1 + (win_y - 1 - nrows) / 2);

    // open up hallways
    int diff;
    for (r=0; r < nrows; r++) {
        for (c=0; c < ncols; c++) {
            cell = grid[r * max_size + c];
            if (cell == 0) {
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, r + r_off, c + c_off, " ");  // wall
                wattroff(win, COLOR_PAIR(1));  // necessary to fix game window box color
            } else if (cell == open_hall) {
                // different colors depending on how far away the torch light reaches
                diff = abs(player[0] - r) + abs(player[1] - c);
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
            } else if (cell == player_posi) {
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, r + r_off, c + c_off, "@");  // player
            }
        }
    }
    wrefresh(win);
}


/**
    Determine if a particular grid cell is a valid move.
 */
bool maze_valid_move(bool maze[], int max_size, int nrows, int ncols, int r, int c) {
    if (r < 0 || c < 0) {
        return false;
    } else if (r >= nrows) {
        return false;
    } else if (c >= ncols) {
        return false;
    } else {
        return !maze[r * max_size + c];
    }
}

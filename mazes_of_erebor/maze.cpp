
#include <iostream>
#include <ncurses.h>
#include <stack>
#include <algorithm>
#include <random>
#include "data.h"

using namespace std;

// forward declarations
static void get_new_dims(int& nrows, int& ncols, int level);
void backtracking_maze_gen(maze_data *maze);
void gen_entrances_opposites(maze_data *maze);


/**
    Get an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
bool maze_get(const maze_data maze, const int row, const int col)
{
    return maze.grid[col + row * maze.ncols];
}


/**
    Set an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
void maze_set(maze_data *maze, const int row, const int col, const bool value)
{
    maze->grid[col + row * maze->ncols] = value;
}


/**
    Find a random, un-opened neighbor of a maze cell.
*/
int* find_neighbor(const maze_data maze, const int row, const int col, int result[])
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
    fill_n(maze->grid, maze->nrows * maze->ncols, true);

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

    maze->grid[maze->start[0] * maze->ncols + maze->start[1]] = false;
    maze->grid[maze->finish[0] * maze->ncols + maze->finish[1]] = false;
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
        return !maze.grid[r * maze.ncols + c];
    }
}


/**
 *   Randomly generate maze dimensions.
 */
static void get_new_dims(int& nrows, int& ncols, int level) {
    level %= 20;

    // min y dim is MAX_DIM / 4 (round up to be odd)
    const int bottom_y((MAX_MAZE_SIZE / 4) % 2 == 0 ? 1 + MAX_MAZE_SIZE / 4 : MAX_MAZE_SIZE / 4);
    nrows = bottom_y + level / 2 + (rand() % (int)(level / 2 + 1));
    if (nrows % 2 == 0) { nrows += 1; }

    // min x dim is twice min y dim (plus one to make it odd)
    const int bottom_x(1 + 2 * bottom_y);
    ncols = bottom_x + level + (int)(rand() % (level + 1));
    if (ncols % 2 == 0) { ncols += 1; }
}


/**
 *   Pull everything together and generate a new maze.
 */
void gen_new_maze(maze_data *maze) {
    get_new_dims(maze->nrows, maze->ncols, maze->level);
    delete[] maze->grid;
    maze->grid = new bool[maze->nrows * maze->ncols];
    backtracking_maze_gen(maze);
    gen_entrances_opposites(maze);
}

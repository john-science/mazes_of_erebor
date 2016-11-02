
#include <stack>
#include <algorithm>
#include <random>
#include "maze.h"

using namespace std;


Maze::Maze() {
    nrows = 19;
    ncols = 31;
    level = -1;
    start[0] = 1;
    start[1] = 1;
    finish[0] = 1;
    finish[1] = 1;
    grid = new bool[nrows * ncols];
}


/**
    Get an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
bool Maze::get(const int row, const int col) const
{
    return grid[col + row * ncols];
}


/**
    Set an element from the maze array.
    (This handles the math to treat a 1D array as a 2D array.)
*/
void Maze::set(const int row, const int col, const bool value)
{
    grid[col + row * ncols] = value;
}


/**
    Find a random, un-opened neighbor of a maze cell.
*/
int* Maze::find_neighbor(const int row, const int col, int result[]) const
{
    int order[4] = {0, 1, 2, 3};
    result[0] = -999;
    result[1] = -999;
    random_shuffle(&order[0], &order[4]);

    for (int i = 0; i < 4; i++){
        if (order[i] == 0){
            if (row > 1 && get(row - 2, col)){
                result[0] = row - 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 1){
            if (row < nrows - 2 && get(row + 2, col)) {
                result[0] = row + 2;
                result[1] = col;
                return result;
            }
        } else if (order[i] == 2) {
            if (col > 1 && get(row, col - 2)) {
                result[0] = row;
                result[1] = col - 2;
                return result;
            }
        } else {
            if (col < ncols - 2 && get(row, col + 2)) {
                result[0] = row;
                result[1] = col + 2;
                return result;
            }
        }
    }

    return result;
}


/**
 *  Determine if a particular grid cell is a valid move.
 */
bool Maze::is_valid_move(const int r, const int c) const {
    if (r < 0 || c < 0) {
        return false;
    } else if (r >= nrows) {
        return false;
    } else if (c >= ncols) {
        return false;
    } else {
        return !grid[r * ncols + c];
    }
}


/**
 *  Generate a maze using the backtracking algorithm.
 */
void Maze::backtracking_gen() {
    int row, col, r, c;
    int neighbor[2] = {-999, -999};
    stack <int> track;

    // ensure maze is correctly initialized
    fill_n(grid, nrows * ncols, true);

    // pick a random starting point
    row = 1 + 2 * (rand() % ((nrows - 1) / 2));
    col = 1 + 2 * (rand() % ((ncols - 1) / 2));
    track.push(row);
    track.push(col);
    set(row, col, false);

    while (track.size() > 0) {
        // choose a grid cell from the track
        col = track.top();
        track.pop();
        row = track.top();
        track.pop();

        // get open neighbors in random order
        find_neighbor(row, col, neighbor);

        if (neighbor[0] > 0) {
            r = neighbor[0];
            c = neighbor[1];

            // dig a hallway to it & add it to frontier
            set(r, c, false);
            set((row + r) / 2, (col + c) / 2, false);
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
void Maze::gen_entrances_opposites()
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

    grid[start[0] * ncols + start[1]] = false;
    grid[finish[0] * ncols + finish[1]] = false;
}


/**
 *   Randomly generate maze dimensions.
 */
void Maze::get_new_dims() {
    const int lev(level % 20);

    // min y dim is MAX_DIM / 4 (round up to be odd)
    const int bottom_y((MAX_MAZE_SIZE / 4) % 2 == 0 ? 1 + MAX_MAZE_SIZE / 4 : MAX_MAZE_SIZE / 4);
    nrows = bottom_y + lev / 2 + (rand() % (int)(lev / 2 + 1));
    if (nrows % 2 == 0) { nrows += 1; }

    // min x dim is twice min y dim (plus one to make it odd)
    const int bottom_x(1 + 2 * bottom_y);
    ncols = bottom_x + lev + (int)(rand() % (lev + 1));
    if (ncols % 2 == 0) { ncols += 1; }
}


/**
 *   Pull everything together and generate a new maze.
 */
void Maze::gen_new() {
    get_new_dims();
    delete[] grid;
    grid = new bool[nrows * ncols];
    backtracking_gen();
    gen_entrances_opposites();
}


/**
 *   Pull everything together and generate the final level.
 *   (Not a maze.)
 */
void Maze::gen_final() {
    // create maze of maximum size
    ncols = MAX_MAZE_SIZE;
    nrows = MAX_MAZE_SIZE / 2;
    // fill maze grid will walls
    delete[] grid;
    grid = new bool[nrows * ncols];
    fill_n(grid, nrows * ncols, true);
    // halways going from left to right
    int start_row(nrows / 2);
    for (int i=0; i < ncols - 1; ++i) {
        set(start_row - 1, i, false);
        set(start_row, i, false);
        set(start_row + 1, i, false);
    }
    set(start_row, ncols - 1, false);
    // start and finish points at opposite ends of the hallway
    start[0] = start_row;
    start[1] = 0;
    finish[0] = start_row;
    finish[1] = ncols - 1;
}

#include <iostream>
#include <stack>
#include <algorithm>
#include <random>

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
    int row = 1;
    int col = 1;
    int r = 1;
    int c = 1;
    int neighbor[2] = {-999, -999};
    stack <int> track;

    // ensure maze is correctly initialized
    fill_n(maze, max_size * max_size, true);

    // pick a random starting point
    srand(time(0));  // TODO: Move to main? What's the difference?
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
void gen_entrances_opposites(int start[], int finish[], int nrows, int ncols)
{
    int wall = 0;

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
}


/**
    Print a maze, including start/finish positions.
*/
void maze_print(bool maze[], int max_size, int nrows, int ncols, int start[], int finish[])
{
    // If we are using ncurses, this should be some sort of mutable buffer.
    char grid[nrows * (ncols + 1) - 1];
    fill_n(grid, nrows * (ncols + 1) - 1, '#');

    // fill in end-of-lines
    for (int i=0; i < nrows; i++) {
        grid[i * (ncols + 1) - 1] = '\n';
    }

    // open up hallways
    for (int r=0; r < nrows; r++) {
        for (int c=0; c < ncols; c++) {
            if (!maze[c + r * max_size]){
                grid[c + r * (ncols + 1)] = ' ';
            }
        }
    }

    // add start and end
    grid[start[1] + start[0] * (ncols + 1)] = ' ';
    grid[finish[1] + finish[0] * (ncols + 1)] = ' ';

    // do the actual printing
    cout << endl << string(grid) << endl;
}

#include <iostream>
#include <ncurses.h>
#include "maze.h"

using namespace std;


int main()
{
    const int MAX_SIZE = 71;
    bool maze[MAX_SIZE * MAX_SIZE];
    int nrows = 19;
    int ncols = 31;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};
    char c;

    initscr();

    while (true) {
        // generate a new maze
        backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
        gen_entrances_opposites(start, finish, nrows, ncols);
        maze_print(maze, MAX_SIZE, nrows, ncols, start, finish);

        // parse user input
        c = getch();
        if (c == 'q'){
            break;
        }

        // increase the size of the maze (to a limit)
        if (ncols < MAX_SIZE) {
            ncols += 2;
        }
        if (nrows < (MAX_SIZE / 2)) {
            nrows += 2;
        }
    }
    endwin();

    return 0;
}

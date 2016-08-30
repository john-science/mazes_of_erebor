
#include "maze.h"


int main()
{
    const int MAX_SIZE = 101;
    bool maze[MAX_SIZE * MAX_SIZE];
    int nrows = 19;
    int ncols = 31;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};

    backtracking_maze_gen(maze, MAX_SIZE, nrows, ncols);
    gen_entrances_opposites(start, finish, nrows, ncols);
    maze_print(maze, MAX_SIZE, nrows, ncols, start, finish);

    return 0;
}

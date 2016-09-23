
#ifndef MAZE_H
#define MAZE_H

const int MAX_MAZE_SIZE(71);

struct maze_data {
    bool grid[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
    int nrows = 19;
    int ncols = 31;
    int max_size = MAX_MAZE_SIZE;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};
};

bool maze_get(const maze_data maze, int row, int col);
void maze_set(maze_data *maze, int row, int col, bool value);
int* find_neighbor(const maze_data maze, int row, int col, int result[]);
void backtracking_maze_gen(maze_data *maze);
void gen_entrances_opposites(maze_data *maze);
bool maze_valid_move(const maze_data maze, int r, int c);
void maze_print_easy(WINDOW *win, const maze_data maze, const int player[]);
void maze_print_medium(WINDOW *win, const maze_data maze, const bool visited[], const int player[]);
void maze_print_hard(WINDOW *win, const maze_data maze, const int player[]);

#endif

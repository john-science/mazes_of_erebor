
#ifndef MAZE_H
#define MAZE_H

bool maze_get(bool maze[], int max_size, int row, int col);
void maze_set(bool maze[], int max_size, int row, int col, bool value);
int* find_neighbor(bool maze[], int nrows, int ncols, int row, int col, int max_size, int result[]);
void backtracking_maze_gen(bool maze[], int max_size, int nrows, int ncols);
void gen_entrances_opposites(int start[], int finish[], int nrows, int ncols);
void maze_print(bool maze[], int max_size, int nrows, int ncols, int start[], int finish[]);

#endif

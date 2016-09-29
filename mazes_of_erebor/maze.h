
#ifndef MAZE_H
#define MAZE_H

#include "data.h"

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

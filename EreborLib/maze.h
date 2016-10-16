
#ifndef MAZE_H
#define MAZE_H

#include "data.h"

// TODO: Move MazeData class definition here.

bool maze_get(const MazeData maze, const int row, const int col);
void maze_set(MazeData *maze, const int row, const int col, const bool value);
int* find_neighbor(const MazeData maze, const int row, const int col, int result[]);
bool maze_valid_move(const MazeData maze, const int r, const int c);
void gen_new_maze(MazeData *maze);
void backtracking_maze_gen(MazeData *maze);  // TODO: Move to an algorithms.cpp

#endif

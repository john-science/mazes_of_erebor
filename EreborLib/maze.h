
#ifndef MAZE_H
#define MAZE_H


const int MAX_MAZE_SIZE(71);


class Maze {
public:
    // attributes
    // TODO: Most of these should be private
    int nrows;
    int ncols;
    int level;
    int start[2];
    int finish[2];
    // methods
    Maze();  // standard initializer
    bool get(int, int) const;
    void set(int, int, bool);
    int* find_neighbor(const int row, const int col, int result[]) const;
    bool is_valid_move(const int, const int) const;
    void gen_new();
    void gen_final();
private:
    // attributes
    bool *grid;
    // methods
    void backtracking_gen();
    void get_new_dims();
    void gen_entrances_opposites();
};


#endif

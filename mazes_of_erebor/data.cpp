
#include <string>

using namespace std;


const int MAX_MAZE_SIZE(71);


enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


struct maze_data {
    bool grid[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
    int level = -1;
    menu_state difficulty = game_easy;
    int nrows = 19 < MAX_MAZE_SIZE ? 19 : MAX_MAZE_SIZE;
    int ncols = 31 < MAX_MAZE_SIZE ? 31 : MAX_MAZE_SIZE;
    int max_size = MAX_MAZE_SIZE;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};
};


struct player_data {
    int loc[2] = {1, 1};
    string name = "Khorin";
    string parent1 = "Balin";  // dwarf
    string parent2 = "Rogyr";  // human
    bool visited[MAX_MAZE_SIZE * MAX_MAZE_SIZE / 2];
};


struct game_data {
    maze_data maze;
    player_data player;
};



#include <string>

using namespace std;


const int MIN_WINDOW_SIZE(13);
const int MAX_MAZE_SIZE(71);


enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


struct maze_data {
    int nrows = 19;
    int ncols = 31;
    bool *grid = new bool[nrows * ncols];
    int level = -1;
    menu_state difficulty = game_easy;
    int start[2] = {1, 1};
    int finish[2] = {1, 1};
};


struct player_data {
    int loc[2] = {1, 1};
    string name = "Khorin";
    string parent1 = "Balin";  // dwarf
    string parent2 = "Rogyr";  // human
    bool *visited = new bool[1];
};


struct game_data {
    maze_data maze;
    player_data player;
};


/**
 *   Init the maze and player location
 */
void reset_player(player_data *player, maze_data *maze) {
    delete[] player->visited;
    player->visited = new bool[maze->nrows * maze->ncols];
    std::fill_n(player->visited, maze->nrows * maze->ncols, false);
    player->visited[maze->finish[0] * maze->ncols + maze->finish[1]] = true;
    player->loc[0] = maze->start[0];
    player->loc[1] = maze->start[1];
}

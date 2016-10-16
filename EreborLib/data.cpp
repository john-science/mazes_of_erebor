
#include <string>
#include "data.h"

using namespace std;

/**
const int MAX_MAZE_SIZE(71);


// TODO: break into game states and menu states (which can be in the other project)
enum menu_state {menu_main, menu_diff, menu_cont,
                 game_easy, game_medium, game_hard,
                 too_small, quit};


enum game_state {game_intro, game_play, game_leveled};
*/

// http://stackoverflow.com/a/865972/1287593
// TODO: Make a lot more of these things private.
/**
class MazeData {
public:
    MazeData();  // standard initializer
    int nrows;
    int ncols;
    bool *grid;
    int level;
    menu_state difficulty;
    int start[2];
    int finish[2];
};
*/


MazeData::MazeData() {
    nrows = 19;
    ncols = 31;
    grid = new bool[nrows * ncols];
    level = -1;
    difficulty = game_easy;
    start[0] = 1;
    start[1] = 1;
    finish[0] = 1;
    finish[1] = 1;
}

/**
class PlayerData {
public:
    PlayerData();
    int loc[2];
    string name;
    string parent1;  // dwarf
    string parent2;  // human
    bool *visited;
};
*/


PlayerData::PlayerData() {
    loc[0] = 1;
    loc[1] = 1;
    name = "Khorin";
    parent1 = "Balin";  // dwarf
    parent2 = "Rogyr";  // human
    visited = new bool[1];
}


/**
class GameData {
public:
    GameData();
    MazeData maze;
    PlayerData player;

    void restart_level();
};
*/


GameData::GameData() {
    maze.level = -1;  // TODO: Just to have something here...
    //maze = MazeData();
    //player = PlayerData();
}


/**
 *   Init the maze and player location
 */
void GameData::restart_level() {
    // reset player
    delete[] player.visited;
    player.visited = new bool[maze.nrows * maze.ncols];
    std::fill_n(player.visited, maze.nrows * maze.ncols, false);
    player.visited[maze.finish[0] * maze.ncols + maze.finish[1]] = true;
    player.loc[0] = maze.start[0];
    player.loc[1] = maze.start[1];
}

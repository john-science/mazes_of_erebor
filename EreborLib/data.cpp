
#include <string>
#include "data.h"

using namespace std;


GameData::GameData() {
    difficulty = game_easy;
    win_level = 100;
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


/**
 *  helper function to determine if you have won the game.
 */
bool GameData::reached_end() {
    if (maze.level == win_level) {
        return true;
    } else {
        return false;
    }
}

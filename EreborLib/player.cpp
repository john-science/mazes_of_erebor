
#include <random>
#include <string>
#include "player.h"


/**
 *  Default initializer
 */
PlayerData::PlayerData() {
    loc[0] = 1;
    loc[1] = 1;
    visited = new bool[1];
}

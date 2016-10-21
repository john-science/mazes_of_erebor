
#ifndef PLAYER_H
#define PLAYER_H

#include <string>


class PlayerData {
public:
    PlayerData();
    int loc[2];
    std::string name;
    std::string parent1;  // dwarf
    std::string parent2;  // human
    bool *visited;
    void reset_names();
};


#endif

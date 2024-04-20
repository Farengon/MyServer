#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>

class Player {
    std::string name;
    int chips;
    int cards[2];
    bool ready;
public:
    Player();
    ~Player();
    void bet();
    int* getHandCards();
};

#endif
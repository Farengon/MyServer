#ifndef __TABLE_H__
#define __TABLE_H__

#include "player.h"
#include <vector>

class Table {
    std::vector<Player> players;
    std::vector<bool> occupied;

    int button = -1;

    void bet(int pos);
public:
    Table(int p_num = 9);
    ~Table();
    void addPlayer(Player player, int pos);
    void removePlayer(int pos);
    void play();
};

#endif
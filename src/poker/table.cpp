#include "table.h"

Table::Table(int p_num = 9) {
    players.resize(p_num);
    occupied.resize(p_num, false);
}

Table::~Table() {}

void Table::addPlayer(Player player, int pos) {
    players[pos] = player;
    occupied[pos] = true;
}

void Table::removePlayer(int pos) {
    occupied[pos] = false;
}

void Table::bet(int pos) {
    while (true) {
        if (occupied[pos]) {
            
        }
    }
}

void Table::play() {

}
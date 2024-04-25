#include "layout.h"

void PokerLayout::start(const std::string& msg) {
    draw();
}

void PokerLayout::update(const std::string& msg) {
    draw();
}

std::string PokerLayout::genMessage() {
    move(getmaxy(stdscr) - 1, 0);
    deleteln();
    std::string input_info = std::string(input_buffer, input_index);
    input_index = 0;
    memset(input_buffer, 0, sizeof(input_buffer));
    return input_info;
}

void PokerLayout::draw() {
    clear();

    if (active) {
        mvprintw(getmaxy(stdscr) - 2, 0, "%s", "Chat:");
    }
    else {
        mvprintw(getmaxy(stdscr) - 2, 0, "%s", "Option: (r+number: Raise to; c: Call; f: Fold)");
    }
    
    mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);

    refresh();
}
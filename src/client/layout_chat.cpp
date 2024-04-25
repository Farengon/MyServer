#include "layout.h"

void ChatLayout::start(const std::string& msg) {
    chat_history.clear();
    draw();
}

void ChatLayout::update(const std::string& msg) {
    chat_history.push_back(msg);
    draw();
}

std::string ChatLayout::genMessage() {
    move(getmaxy(stdscr) - 1, 0);
    deleteln();
    std::string input_info = std::string(input_buffer, input_index);
    input_index = 0;
    memset(input_buffer, 0, sizeof(input_buffer));
    return input_info;
}

void ChatLayout::draw() {
    clear();

    int line = std::min(int(chat_history.size() - 1), getmaxy(stdscr) - 3);
    for (int i = chat_history.size() - 1; i >= 0 && line >= 0; --i) {
        move(line--, 0);
        clrtoeol();
        printw("%s", chat_history[i].c_str());
    }
    mvprintw(getmaxy(stdscr) - 2, 0, "%s", "Send:");
    mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);

    refresh();
}
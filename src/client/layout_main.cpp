#include "layout.h"

void MainLayout::start(const std::string& msg) {
    draw();
}

void MainLayout::update(const std::string& msg) {
    draw();
}

std::string MainLayout::genMessage() {
    move(getmaxy(stdscr) - 1, 0);
    deleteln();
    std::string input_info = std::string(input_buffer, input_index);
    input_index = 0;
    memset(input_buffer, 0, sizeof(input_buffer));
    return "chmod|" + input_info;
}

void MainLayout::draw() {
    clear();

    for (int i = 0; i < prompt_num; i++) {
        mvprintw(i, 0, "%s", prompts[i].c_str());
    }

    mvprintw(getmaxy(stdscr) - 2, 0, "%s", "Chose a func:");
    mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);

    refresh();
}
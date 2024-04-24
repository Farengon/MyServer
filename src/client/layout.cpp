#include "layout.h"

void Layout::input(const char ch) {
    if (input_index < sizeof(input_buffer)) {
        input_buffer[input_index++] = ch;
        mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);
        refresh();
    }
}

void Layout::resize() {
    endwin();
    refresh();
    initScreen();
    draw();
    refresh();
}

void Layout::initScreen() {
    // Initialize curses
    initscr(); // Initialize curses mode
    clear();
    cbreak(); // Disable line buffering
    noecho(); // Do not echo input characters
    keypad(stdscr, true);

    draw();
}
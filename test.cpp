#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>

void handle_resize(int sig) {
    endwin(); // End ncurses mode
    refresh(); // Refresh screen
    // Reinitialize ncurses to adapt to the new terminal size
    initscr();
    clear(); // Clear the screen
    printw("Terminal resized. Press any key to continue.\n");
    refresh();
}

int main() {
    // Initialize ncurses
    initscr();
    clear(); // Clear the screen
    printw("Welcome! Resize the terminal window to test input.\n");
    refresh();

    // Set up signal handler for SIGWINCH
    signal(SIGWINCH, handle_resize);

    // Main program loop
    int ch;
    while (1) {
        ch = getch(); // Read a character from stdin
        if (ch != ERR) {
            printw("You pressed: %c\n", ch);
            refresh();
            if (ch == 'q') // Quit on 'q'
                break;
        }
    }

    // End ncurses
    endwin();
    return 0;
}
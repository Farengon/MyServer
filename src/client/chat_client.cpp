#include "chat_client.h"
#include "exception.h"
#include "common.h"
#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <cstring>
#include <algorithm>
#include <signal.h>

fd_set fds;

ChatClient::ChatClient(std::string server_ad, int port): Client(server_ad, port) {}

ChatClient::~ChatClient(){
    endwin();
}

void ChatClient::connectServer() {
    Client::connectServer();
    std::cout << "Enter your name: ";

    std::string name;
    std::getline(std::cin, name);

    write(m_socket, name.c_str(), name.length());
}

void ChatClient::displayChat() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int line = std::min(int(chat_history.size() - 1), max_y - 2);
    for (int i = chat_history.size() - 1; i >= 0 && line >= 0; --i) {
        move(line--, 0);
        clrtoeol();
        printw("%s", chat_history[i].c_str());
    }
    mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);

    refresh();
}

volatile sig_atomic_t resized = 0;

void initScreen() {
    // Initialize curses
    initscr(); // Initialize curses mode
    clear();
    cbreak(); // Disable line buffering
    noecho(); // Do not echo input characters

    int max_y = getmaxy(stdscr);

    // mvprintw(max_y - 1, 0, "Welcome! %d\n", max_y);
    refresh();

    move(max_y - 1, 0);
    refresh();
}

void handle_resize(int sig) {
    resized = 1;
}

void ChatClient::handleMessages() {

    signal(SIGWINCH, handle_resize);

    // Initialize curses
    initScreen();

    while (true) {
        if (resized) {
            endwin();
            refresh();

            initScreen();

            displayChat();

            resized = 0;
        }

        refresh();

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(m_socket, &fds);

        int select_res;
        // do {
        select_res = select(m_socket + 1, &fds, nullptr, nullptr, nullptr);
        // } while (select_res < 0 && errno == EINTR); // Here we need to do so cuz resizing modifies stdin.
        if (select_res < 0 && errno == EINTR) {
            continue;
        }

        if (select_res < 0) {
            exitWithError("select failed");
        }

        if (FD_ISSET(STDIN_FILENO, &fds) && select_res >= 0) {
            char ch;
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                if (ch == '\r' || ch == '\n') {
                    deleteln();
                    move(getmaxy(stdscr) - 1, 0);
                    input_buffer[input_index] = '\0';
                    if (input_index > 0) {
                        write (m_socket, input_buffer, input_index);
                        input_index = 0;
                        memset(input_buffer, 0, sizeof(input_buffer));
                    }
                }
                else if (input_index < sizeof(input_buffer)) {
                    input_buffer[input_index++] = ch;
                    mvprintw(getmaxy(stdscr) - 1, 0, "%s", input_buffer);
                }
            }
        }

        if (FD_ISSET(m_socket, &fds)) {
            char buffer[1024];
            int msg_len = read(m_socket, buffer, sizeof(buffer));
            if (msg_len > 0) {
                buffer[msg_len] = '\0';
                chat_history.push_back(buffer);
                displayChat();
            }
            else if (msg_len == 0) {
                std::cout << "disconnected" << std::endl;
                break;
            }
            else {
                exitWithError("read error");
            }
        }
    }
}
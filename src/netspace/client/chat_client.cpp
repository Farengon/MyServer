#include "chat_client.h"
#include "exception.h"
#include "common.h"
#include <iostream>
#include <unistd.h>
#include <curses.h>
#include <cstring>

Chat_client::Chat_client(std::string server_ad, int port): Client(server_ad, port) {}

Chat_client::~Chat_client(){
    endwin();
}

void Chat_client::connectServer() {
    Client::connectServer();
    char prompt[1024];
    read(m_socket, prompt, sizeof(prompt));
    std::cout << prompt;

    std::string name;
    std::getline(std::cin, name);

    write(m_socket, name.c_str(), name.length());
}

void Chat_client::handleMessages() {
    // Initialize curses
    initscr(); // Initialize curses mode
    cbreak(); // Disable line buffering
    noecho(); // Do not echo input characters

    printw("Welcome!\n");
    refresh();

    int max_y = getmaxy(stdscr);
    printw("%d\n", max_y);
    refresh();

    move(max_y - 1, 0);
    refresh();

    while (true) {
        refresh();

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(m_socket, &fds);

        if (select(m_socket + 1, &fds, nullptr, nullptr, nullptr) < 0) {
            exitWithError("select failed");
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
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
            // std::string msg;
            // std::getline(std::cin, msg);
            // write (m_socket, msg.c_str(), msg.length());
        }

        if (FD_ISSET(m_socket, &fds)) {
            char buffer[1024];
            int msg_len = read(m_socket, buffer, sizeof(buffer));
            if (msg_len > 0) {
                buffer[msg_len] = '\0';
                std::cout << GREEN << buffer << RESET << std::endl;
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
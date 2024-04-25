#include "client.h"
#include "exception.h"
#include "common.h"
#include <unistd.h>
#include <iostream>
#include <signal.h>

const int BUFFER_SIZE = 1024;

Client::Client(std::string server_ad, int port) {
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        exitWithError("socket error");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ad.c_str(), &server_addr.sin_addr) <= 0) {
        exitWithError("invalid server address");
    }

    layout = layouts[0];
}

Client::~Client() {
    close(m_socket);
    for (auto l: layouts) {
        delete l;
    }
    endwin();
}

void Client::startConnect() {
    connectServer();
    handleMessages();
}

void Client::connectServer() {
    if (connect(m_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        exitWithError("connect failed");
    }
    std::cout << "connected" << std::endl;

    std::cout << "Enter your name: ";

    std::string name;
    std::getline(std::cin, name);

    write(m_socket, name.c_str(), name.length());
}

volatile sig_atomic_t resized = 0;

void handle_resize(int sig) {
    resized = 1;
}

void Client::handleMessages() {

    signal(SIGWINCH, handle_resize);

    initscr(); // Initialize curses mode
    clear();
    cbreak(); // Disable line buffering
    noecho(); // Do not echo input characters
    keypad(stdscr, true);
    layout->start("");
    
    while(true) {
        if (resized) {
            layout->resize();
            resized = 0;
        }
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(m_socket, &fds);

        int select_res;
        select_res = select(m_socket + 1, &fds, nullptr, nullptr, nullptr);
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
                    std::string msg = layout->genMessage();
                    if (msg.length() > 0) {
                        write (m_socket, msg.c_str(), msg.length());
                    }
                }
                else if (ch == 27) {
                    // ESC
                    if (mod == 0) {
                        break;
                    }
                    else {
                        write(m_socket, "chmod|0", 7);
                    }
                }
                else {
                    layout->input(ch);
                }
            }
        }

        if (FD_ISSET(m_socket, &fds)) {
            char buffer[1024];
            int msg_len = read(m_socket, buffer, sizeof(buffer));
            std::string msg(buffer, msg_len);
            
            if (msg_len > 0) {
                std::vector<std::string> tokens = splitString(msg, '|');
                if (tokens[0] == "chmod") {
                    // if tokens[1].length != 1 >> exception;
                    mod = tokens[1][0] - '0';
                    layout = layouts[mod];
                    endwin();
                    layout->start(msg);
                }
                else {
                    layout->update(msg);
                }
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
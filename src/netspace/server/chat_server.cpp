#include "chat_server.h"
#include <unistd.h>
#include <iostream>

const int BUFFER_SIZE = 1024;

Chat_server::Chat_server(std::string ip_address, int port): Server(ip_address, port) {}

Chat_server::~Chat_server() {}

int Chat_server::acceptNewConnection() {
    int client_fd = Server::acceptNewConnection();
    write(client_fd, "Enter your name: ", 18);
    return client_fd;
}

void Chat_server::handleClientData(const int fd) {
    char buffer[BUFFER_SIZE];
    int msg_len = read(fd, buffer, BUFFER_SIZE);
    if (msg_len <= 0) {
        epoll_ctl(ep_fd, EPOLL_CTL_DEL, fd, nullptr);
        close(fd);
        client_names.erase(fd);
        std::cout << "disconnected" << std::endl;
    }
    else {
        if (client_names.find(fd) == client_names.end()) {
            client_names[fd] = std::string(buffer, msg_len);
            std::cout << "client " << client_names[fd] << " is connected" << std::endl;
        }
        else {
            std::string msg = client_names[fd] + ": " + std::string(buffer, msg_len);
            for (auto& client : client_names) {
                if (client.first != fd) {
                    write(client.first, msg.c_str(), msg.length());
                }
            }
        }
    }
}

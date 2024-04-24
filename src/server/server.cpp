#include "server.h"
#include "exception.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

const int BUFFER_SIZE = 1024;
const int EPOLL_SIZE = 1024;

std::vector<std::string> Server::splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input); // Use a stringstream to parse the input string
    std::string token;

    // Tokenize the string using the specified delimiter
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int Server::createServer() {
    std::cout << "create" << std::endl;
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exitWithError("Can not open socket");
    }

    if (bind(m_socket,(sockaddr *)&m_socket_address, m_socket_address_len) < 0)
    {
        exitWithError("Can not connect socket to address");
    }

    return 0;
}

int Server::closeServer() {
    close(m_socket);
    return 0;
}

Server::Server(std::string ip_address, int port): m_ip_address(ip_address), m_port(port), m_socket_address_len(sizeof(m_socket_address)) 
{
    std::cout << "server" << std::endl;
    m_socket_address.sin_family = AF_INET;
    m_socket_address.sin_port = htons(m_port);
    m_socket_address.sin_addr.s_addr = INADDR_ANY;

    for (auto state = MAIN; state <= END; state = (States)(state+1)) {
        state_sets.push_back({});
    }

    createServer();
}

Server::~Server() {
    closeServer();
}

void Server::startListen() {
    if (listen(m_socket, 10) < 0) {
        exitWithError("Listen failed");
    }

    std::cout << "Server start listen" << std::endl;

    if ((ep_fd = epoll_create(1)) < 0) {
        exitWithError("epoll create error");
    }

    event.events = EPOLLIN;
    event.data.fd = m_socket;

    if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, m_socket, &event) < 0) {
        exitWithError("epoll ctl error");
    }

    while (true) {
        int ev_num = epoll_wait(ep_fd, events, MAX_EVENTS, -1);
        if (ev_num == -1) {
            exitWithError("epoll wait error");
        }

        for (int i = 0; i < ev_num; i++) {
            if (events[i].data.fd == m_socket) {
                // new connection
                acceptNewConnection();
            }
            else {
                handleClientData(events[i].data.fd);
            }
        }
    }

}

int Server::acceptNewConnection() {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(m_socket, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        exitWithError("accept error");
    }
    else {
        std::cout << "new connection" << std::endl;

        // ser non-blocking
        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

        event.events = EPOLLIN | EPOLLET;
        event.data.fd = client_fd;
        if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
            exitWithError("epoll ctl client error");
        }
    }
    return client_fd;
}

void Server::handleClientData(const int fd) {
    char buffer[BUFFER_SIZE];
    int msg_len = read(fd, buffer, BUFFER_SIZE);
    if (msg_len <= 0) {
        epoll_ctl(ep_fd, EPOLL_CTL_DEL, fd, nullptr);
        close(fd);
        client_names.erase(fd);
        state_sets[client_states[fd]].erase(fd);
        client_states.erase(fd);
        std::cout << "disconnected" << std::endl;
    }
    else {
        std::string msg(buffer, msg_len);
        std::cout << msg << std::endl;
        if (client_names.find(fd) == client_names.end()) {
            client_names[fd] = msg;
            client_states[fd] = MAIN;
            state_sets[MAIN].insert(fd);
            std::cout << "client " << client_names[fd] << " is connected" << std::endl;
        }
        else {
            std::vector<std::string> tokens = splitString(msg, '|');
            if (tokens[0] == "chmod") {
                int mod = tokens[1][0] - '0';
                client_states[fd] = mod;
                state_sets[mod].insert(fd);
                write(fd, buffer, msg_len);
            }
            else {
                std::string res = client_names[fd] + "|" + msg;
                for (auto& client: state_sets[client_states[fd]]) {
                    write(client, res.c_str(), res.length());
                }
            }
        }
    }
}
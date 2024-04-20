#include "server.h"
#include "exception.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

const int BUFFER_SIZE = 1024;
const int EPOLL_SIZE = 1024;


int Server::createServer() {
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
    exit(0);
}

Server::Server(std::string ip_address, int port): m_ip_address(ip_address), m_port(port), m_socket_address_len(sizeof(m_socket_address)) 
{
    m_socket_address.sin_family = AF_INET;
    m_socket_address.sin_port = htons(m_port);
    m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

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
        std::cout << "disconnected" << std::endl;
    }
    else {
        write(fd, buffer, msg_len);
    }
}
#ifndef __SERVER_H__
#define __SERVER_H__

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>
#include <sys/epoll.h>

const int MAX_EVENTS = 10;

class Server {
    int m_socket;

    std::string m_ip_address;
    int m_port;

    struct sockaddr_in m_socket_address;
    int m_socket_address_len;

    int createServer();
    int closeServer();

protected:
    int ep_fd;
    epoll_event event, events[MAX_EVENTS];

    virtual int acceptNewConnection();
    virtual void handleClientData(const int fd);
    
public:
    Server(std::string ip_address, int port);
    ~Server();
    void startListen();
};

#endif
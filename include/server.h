#ifndef __SERVER_H__
#define __SERVER_H__

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>
#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
#include <set>

const int MAX_EVENTS = 10;

enum States{
    MAIN,
    CHAT,
    POKER,
    END,
};

class Server {
    int m_socket;

    std::string m_ip_address;
    int m_port;

    struct sockaddr_in m_socket_address;
    int m_socket_address_len;

    int createServer();
    int closeServer();

    std::unordered_map<int, std::string> client_names;
    std::unordered_map<int, int> client_states;

    std::vector<std::set<int>> state_sets;

    std::vector<std::string> splitString(const std::string& input, char delimiter);

    void log(const std::string& msg);

protected:
    int ep_fd;
    epoll_event event, events[MAX_EVENTS];

    virtual int acceptNewConnection();
    virtual void handleClientData(const int fd);
    
public:
    Server(std::string ip_address, int port);
    virtual ~Server();
    void startListen();
};

#endif
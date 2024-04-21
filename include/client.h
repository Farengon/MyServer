#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>

class Client {

protected:
    int m_socket;

    sockaddr_in server_addr;

    virtual void connectServer();
    virtual void handleMessages();

public:
    Client(std::string server_ad, int port);
    virtual ~Client();
    void startConnect();
};

#endif
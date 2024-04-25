#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <vector>
#include "layout.h"

class Client {

    Layout* layout;
    Layout* layouts[2] = {new MainLayout(), new ChatLayout()};

    int mod;


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
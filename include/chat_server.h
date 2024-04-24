#ifndef __CHAT_SERVER_H__
#define __CHAT_SERVER_H__

#include "server.h"
#include <unordered_map>

class ChatServer: public Server
{
private:
    std::unordered_map<int, std::string> client_names;

    int acceptNewConnection();
    void handleClientData(const int fd);
public:
    ChatServer(std::string ip_address, int port);
    ~ChatServer();
};


#endif
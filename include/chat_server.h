#ifndef __POKER_SERVER_H__
#define __CHAT_SERVER_H__

#include "server.h"
#include <unordered_map>

class Chat_server: public Server
{
private:
    std::unordered_map<int, std::string> client_names;

    int acceptNewConnection();
    void handleClientData(const int fd);
public:
    Chat_server(std::string ip_address, int port);
    ~Chat_server();
};


#endif
#ifndef __CHAT_CLIENT_H__
#define __CHAT_CLIENT_H__

#include "client.h"

class Chat_client: public Client {
    void connectServer();
    void handleMessages();
public:
    Chat_client(std::string server_ad, int port);
    ~Chat_client();
};

#endif
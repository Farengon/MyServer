#ifndef __CHAT_CLIENT_H__
#define __CHAT_CLIENT_H__

#include "client.h"

class Chat_client: public Client {
    char input_buffer[1024];
    int input_index = 0;

    void connectServer();
    void handleMessages();
public:
    Chat_client(std::string server_ad, int port);
    ~Chat_client();
};

#endif
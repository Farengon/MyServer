#ifndef __CHAT_CLIENT_H__
#define __CHAT_CLIENT_H__

#include "client.h"
#include <vector>

class Chat_client: public Client {
    char input_buffer[1024];
    int input_index = 0;

    std::vector<std::string> chat_history;

    void connectServer();
    void handleMessages();
    void displayChat();
public:
    Chat_client(std::string server_ad, int port);
    ~Chat_client();
};

#endif
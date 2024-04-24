#include "chat_client.h"

int main() {
    Client* client = new Client("0.0.0.0", 6515);

    client->startConnect();

    delete client;

    return 0;
}
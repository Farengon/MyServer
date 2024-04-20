#include "chat_server.h"
#include <sys/socket.h>

int main() {
    Server* server = new Chat_server("0.0.0.0", 6515);

    server->startListen();

    return 0;
}
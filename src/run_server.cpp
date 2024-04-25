#include "server.h"
#include <sys/socket.h>

int main() {
    Server* server = new Server("0.0.0.0", 6515);

    server->startListen();

    return 0;
}
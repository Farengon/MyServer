#include "client.h"
#include "exception.h"
#include <unistd.h>
#include <iostream>

const int BUFFER_SIZE = 1024;

Client::Client(std::string server_ad, int port) {
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        exitWithError("socket error");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ad.c_str(), &server_addr.sin_addr) <= 0) {
        exitWithError("invalid server address");
    }
}

Client::~Client() {
    close(m_socket);
}

void Client::startConnect() {
    connectServer();
    handleMessages();
}

void Client::connectServer() {
    if (connect(m_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        exitWithError("connect failed");
    }
    std::cout << "connected" << std::endl;
}

void Client::handleMessages() {
    while(true) {
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        write(m_socket, message.c_str(), message.length());

        char buffer[BUFFER_SIZE];
        int msg_len = read(m_socket, buffer, BUFFER_SIZE);
        if (msg_len > 0) {
            buffer[msg_len] = '\0';
            std::cout << "response: " << buffer << std::endl;
        }
    }
}
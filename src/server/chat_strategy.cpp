#include "server_strategy.h"

ServerChatStrategy::ServerChatStrategy() {
    target_fds.clear();
}

std::string ServerChatStrategy::run(const std::string& msg) {
    return msg;
}
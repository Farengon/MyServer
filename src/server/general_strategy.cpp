#include "server_strategy.h"

ServerGeneralStrategy::ServerGeneralStrategy() {
    target_fds.insert(-1);
}

std::string ServerGeneralStrategy::run(const std::string& msg) {
    return msg;
}
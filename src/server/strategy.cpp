#include "server_strategy.h"

const std::set<int>& ServerStrategy::get_targets() {
    return target_fds;
}
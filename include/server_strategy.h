#ifndef __SERVER_STRATEGY_H__
#define __SERVER_STRATEGY_H__

#include "strategy.h"

class ServerStrategy {
    protected:
    std::set<int> target_fds;
public:
    ServerStrategy(){};
    virtual ~ServerStrategy(){};
    virtual std::string run(const std::string& msg) = 0;
    const std::set<int>& get_targets();
};

class ServerGeneralStrategy: public ServerStrategy {
public:
    std::string run(const std::string& msg);
    ServerGeneralStrategy();
};

class ServerChatStrategy: public ServerStrategy {
    ServerChatStrategy();
    std::string run(const std::string& msg);
};

#endif
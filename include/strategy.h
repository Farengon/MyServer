#ifndef __STRATEGY_H__
#define __STRATEGY_H__

#include <string>
#include <set>

class Strategy {
protected:
    std::set<int> target_fds;
public:
    Strategy(){};
    virtual ~Strategy(){};
    virtual std::string run() = 0;
    const std::set<int>& get_targets();
};

#endif
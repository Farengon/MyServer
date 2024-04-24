#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "common.h"

class Layout {
protected:
    char input_buffer[1024];
    int input_index;
public:
    Layout(): input_index(0) {};
    virtual ~Layout(){};
    virtual void draw() = 0;
    virtual std::string genMessage() = 0;
    virtual void start(const std::string& msg) = 0;
    virtual void update(const std::string& msg) = 0;

    void input(const char ch);
    void initScreen();
    void resize();
};

class MainLayout: public Layout {
    const int prompt_num = 2;
    const std::string prompts[2] = {"1. Chat", "2. Poker"};
public:
    MainLayout(){};
    void draw();
    std::string genMessage();
    void start(const std::string& msg);
    void update(const std::string& msg);
};

class ChatLayout: public Layout {
    std::vector<std::string> chat_history;
public:
    ChatLayout(){};
    void draw();
    std::string genMessage();
    void start(const std::string& msg);
    void update(const std::string& msg);
};

#endif
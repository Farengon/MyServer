#ifndef __COMMON_H__
#define __COMMON_H__

#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

std::vector<std::string> splitString(const std::string& input, char delimiter);

#endif
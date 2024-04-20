#include "exception.h"
#include <iostream>

void exitWithError(const std::string &err_msg) {
    std::cerr << err_msg << std::endl;
    exit(1);
}
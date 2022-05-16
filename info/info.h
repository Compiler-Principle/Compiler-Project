#ifndef COMPILER_PROJECT_INFO_H
#define COMPILER_PROJECT_INFO_H
#include <string>
#include <iostream>
#include <string.h>
enum InfoLevel {
    INFO,
    WARNING,
    ERROR
};

void info(InfoLevel level, std::string message);

#endif //COMPILER_PROJECT_INFO_H

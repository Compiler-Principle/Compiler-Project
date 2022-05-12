#include "info.h"


void info(InfoLevel level, std::string message){
    if(level == InfoLevel::INFO){
        std::cout << "[INFO]: " << message << std::endl;
    }
    else if(level == InfoLevel::WARNING){
        std::cout << "[WARNING]: " << message << std::endl;
    }
    else if(level == InfoLevel::ERROR){
        std::cerr << "[ERROR]: " << message << std::endl;
    }
}
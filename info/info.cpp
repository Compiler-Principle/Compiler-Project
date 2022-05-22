#include "info.h"


void info(InfoLevel level, std::string message){
    if(level == InfoLevel::INFO){
        std::cout << "\033[32m[ INFO ]: \033[0m" ;
        std::cout << message << std::endl;
    }
    else if(level == InfoLevel::WARNING){
        std::cout << "\033[33m[ WARNING ]: \033[0m" ;
        std::cout << message << std::endl;
    }
    else if(level == InfoLevel::ERROR){
        std::cout << "\033[31m[ ERROR ]: \033[0m" ;
        std::cout << message << std::endl;
    }
}
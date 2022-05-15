cmake_minimum_required(VERSION 3.20)
project(Compiler_Project)


set(CMAKE_CXX_STANDARD 17)



add_subdirectory(AST)
add_subdirectory(info)


add_executable(${PROJECT_NAME}
        main.cpp
        )

#target_link_libraries(${PROJECT_NAME}
#        PUBLIC AST info
#        )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -g)
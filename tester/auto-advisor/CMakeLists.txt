cmake_minimum_required(VERSION 3.20)
project(auto-advisor)
set(CMAKE_CXX_STANDARD 17)


add_executable(${PROJECT_NAME} aa_nochar.c)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -g )


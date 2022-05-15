cmake_minimum_required(VERSION 3.20)
project(Compiler_Project)
set(CMAKE_CXX_STANDARD 17)


# ---------------- FLEX and BISON ----------------
find_package(BISON)
find_package(FLEX)

FLEX_TARGET(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
        COMPILE_FLAGS "-ll -g")
add_subdirectory(AST)
add_subdirectory(info)
add_flex_bison_dependency(Scanner Parser)
add_executable(${PROJECT_NAME}
        ${Parser_OUTPUTS}
        ${Scanner_OUTPUTS}
        AST/AST.cpp
        info/info.cpp
        main.cpp)

target_link_libraries(${PROJECT_NAME}
        PUBLIC AST info
        )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -g )
set(CMAKE_CXX_FLAGS -ll)

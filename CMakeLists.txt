cmake_minimum_required(VERSION 3.20)
#SET(CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-11")
#SET(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-11")
project(Compiler_Project)
set(CMAKE_CXX_STANDARD 17)


# ---------------- FLEX and BISON ----------------
#find_package(BISON)
#find_package(FLEX)

#FLEX_TARGET(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.c)
#
#BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
#        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
#        COMPILE_FLAGS "-ll -g")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_custom_command(
        OUTPUT lex.yy.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l
        COMMAND flex ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l
)

add_custom_command(
        OUTPUT parser.tab.cpp  parser.tab.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
#        COMMAND bison -d ${CMAKE_CURRENT_SOURCE_DIR}/parser.y -v --debug
        COMMAND bison -d ${CMAKE_CURRENT_SOURCE_DIR}/parser.y --defines=parser.tab.h -o parser.tab.cpp -v --debug
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp
)
add_subdirectory(AST)
add_subdirectory(info)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME}
        parser.tab.cpp
        AST/AST.cpp
        info/info.cpp
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC AST info
        )
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/AST
        ${CMAKE_CURRENT_SOURCE_DIR}/info
        )

#set(CMAKE_CXX_FLAGS -g )
#set(CMAKE_CXX_FLAGS -ll)

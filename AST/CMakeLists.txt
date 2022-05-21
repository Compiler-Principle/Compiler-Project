add_library(AST
        AST.cpp
        )
target_include_directories(AST
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
        )
set_property(TARGET AST PROPERTY CXX_STANDARD 17)


add_library(info
        info.cpp
        )
#target_include_directories(info
#        PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
#        )

set_property(TARGET info PROPERTY CXX_STANDARD 17)
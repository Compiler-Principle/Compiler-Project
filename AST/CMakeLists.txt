cmake_minimum_required(VERSION 3.20)
project(Compiler_Project)

set(CMAKE_CXX_STANDARD 14)

#--------------Add Targets of Project Begin --------------#

add_executable(Compiler_Project
        AST.cpp PainterWindow.cpp )

#--------------Add Targets of Project End --------------#
#include_directories("/opt/homebrew/Cellar/qt@5/5.15.2/include")



#--------------Project Config Begin --------------#




#--------------Qt CMake Config Begin --------------#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)

#   specific the cmake-prefix-path of qt5
set(CMAKE_PREFIX_PATH /opt/homebrew/Cellar/qt@5/5.15.2_1/lib/cmake)
if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

#   select the libs
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
#   link to project
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})


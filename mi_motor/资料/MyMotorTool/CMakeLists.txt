cmake_minimum_required(VERSION 2.8)

PROJECT(MyMotorTool)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON) # for meta object compiler
set(CMAKE_AUTORCC ON) # resource files
set(CMAKE_AUTOUIC ON) # ui files



find_package(Qt5 REQUIRED COMPONENTS Widgets PrintSupport)















file(GLOB SRCS src/*.cpp src/*.ui)


ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS} )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread boost_thread Qt5::Widgets Qt5::PrintSupport stdc++fs )


add_definitions(-O3 -g)

cmake_minimum_required(VERSION 3.0.2)

project(WBTool)

set (SRCS wbtool.cpp main_menu.cpp menu.cpp)

add_executable(wbtool ${SRCS})
target_link_libraries(wbtool menu ncurses)

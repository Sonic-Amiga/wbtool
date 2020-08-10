cmake_minimum_required(VERSION 3.0.2)

project(WBTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(MODBUS REQUIRED libmodbus)

set (SRCS wbtool.cpp device.cpp main_menu.cpp baud_menu.cpp menu.cpp)

add_executable(wbtool ${SRCS})
target_include_directories(wbtool PRIVATE ${MODBUS_INCLUDE_DIRS})
target_link_directories(wbtool PRIVATE ${MODBUS_LIBRARY_DIRS})
target_link_libraries(wbtool menu newt slang modbus)

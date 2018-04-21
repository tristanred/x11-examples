cmake_minimum_required(VERSION 3.9)
project(experiments)

set(CMAKE_CXX_STANDARD 17)

add_executable(experiments main.cpp)

target_link_libraries(experiments X11)
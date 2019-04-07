cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

project (RAW12PPM)

#flags

#include files
include_directories(./include ./libs/Debayer/include)
add_subdirectory(./libs/Debayer)

#target
add_executable(run ./main.cpp ./src/RAW12.cpp ./src/PPM.cpp)

#libs
target_link_libraries(run debayer)

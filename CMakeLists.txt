cmake_minimum_required(VERSION 3.0)

project (RAW12PPM)

#flags

#include files
include_directories(./include ./libs/Debayer/include)

#target
add_executable(run ./src/RAW12.cpp ./src/PPM.cpp)

#libs
add_subdirectory(./libs/Debayer)
target_link_libraries(run Debayer)

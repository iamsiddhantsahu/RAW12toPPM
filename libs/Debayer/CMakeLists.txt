cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

project(Debayer)

#include files
include_directories(./include ../../include)

add_library(debayer SHARED ./src/Debayer.cpp)

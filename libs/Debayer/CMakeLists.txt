cmake_minimum_required(VERSION 3.0)

project(Debayer)

#include files
include_directories(./include)

add_library(debayer SHARED ./src/Debayer.cpp)

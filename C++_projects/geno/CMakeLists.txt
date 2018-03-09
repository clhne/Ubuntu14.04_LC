cmake_minimum_required(VERSION 2.8)
project(Networking3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp networking.cpp networking.h http.cpp http.h)
add_executable(Networking3 ${SOURCE_FILES})
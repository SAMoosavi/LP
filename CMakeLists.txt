cmake_minimum_required(VERSION 3.26)
project(LP)

set(CMAKE_CXX_STANDARD 20)

add_executable(Dual Dual.cpp
        InputLP.cpp
        InputLP.h)

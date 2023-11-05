cmake_minimum_required(VERSION 3.26)
project(LP)

set(CMAKE_CXX_STANDARD 11)

add_executable(Dual Dual.cpp
		InputLP.cpp
		InputLP.h
		ColoredString.cpp
		ColoredString.h
		LP.cpp
		LP.h)

add_executable(Simplex Simplex.cpp
		InputLP.cpp
		InputLP.h
		ColoredString.cpp
		ColoredString.h
		LP.cpp
		LP.h)
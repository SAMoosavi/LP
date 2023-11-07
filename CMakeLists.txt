cmake_minimum_required(VERSION 3.26)
project(LP)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp
		Dual.cpp
		InputLP.cpp
		InputLP.h
		ColoredString.cpp
		ColoredString.h
		LP.cpp
		LP.h
		Simplex.cpp
		Dual.h
		Simplex.h)

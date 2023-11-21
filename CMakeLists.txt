cmake_minimum_required(VERSION 3.26)
project(LP)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
		Dual.cpp
		Dual.h
		InputLP.cpp
		InputLP.h
		ColoredString.cpp
		ColoredString.h
		LP.cpp
		LP.h
		Simplex.h
		Simplex.cpp
		MNumber-tmp.h
		MNumber.h)

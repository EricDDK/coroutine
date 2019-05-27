cmake_minimum_required (VERSION 2.6)
add_compile_options(-std=c++11)
project (coroutine)

add_executable(
    main 
	Common.h
	Coroutine.h
	Ordinator.h
	Routine.h
    Test.cpp
)


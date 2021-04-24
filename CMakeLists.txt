# cmake_minimum_required(VERSION <specify CMake version here>)
project(chese C)

set(CMAKE_C_STANDARD 99)

add_executable(chese main.c chess.h chess.c)
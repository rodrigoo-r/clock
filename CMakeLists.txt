cmake_minimum_required(VERSION 3.30)
project(clock C)

set(CMAKE_C_STANDARD 11)

add_library(clock STATIC library.c)

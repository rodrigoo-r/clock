cmake_minimum_required(VERSION 3.12)
project(clock C)

set(CMAKE_C_STANDARD 11)

add_library(clock STATIC clock.c clock.h)

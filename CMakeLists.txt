cmake_minimum_required(VERSION 3.20)
project(aoc2021 C)

set(CMAKE_C_STANDARD 11)

add_executable(aoc2021 main.c util.c util.h day01.c day01.h day02.c day02.h)

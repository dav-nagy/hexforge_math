cmake_minimum_required(VERSION 3.31)
project(math)

set(CMAKE_CXX_STANDARD 20)

add_executable(math main.cpp
        math/f32/f32.h
        math/f32/f32.cpp)

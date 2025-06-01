cmake_minimum_required(VERSION 3.31)
project(math2)

set(CMAKE_CXX_STANDARD 20)

add_executable(math2 main.cpp
        math/f32/f32.h
        math/f32/implement/f32.cpp
        math/char/char.h
        math/char/implement/char.cpp
        math/char/nan_helper.h
        math/f32/implement/f32_functions.cpp
        math/f32/f32_functions.h
)

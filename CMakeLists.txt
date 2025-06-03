cmake_minimum_required(VERSION 3.31)
project(math)

set(CMAKE_CXX_STANDARD 20)

add_executable(math main.cpp
        math/f32/f32.h
        math/f32/implement/f32.cpp
        math/char/char.h
        math/char/implement/char.cpp
        math/char/nan_helper.h
        math/f32/implement/f32_functions.cpp
        math/f32/f32_functions.h
        math/f32/implement/snan_err.cpp
        math/f32/snan_err.h
        io/input_output/write/implement/str_write.cpp
        io/input_output/write/str_write.h
        string/implement/to_string.cpp
        string/to_string.h
        string/implement/string.cpp
        string/string.h
        math/f32/implement/trunc.cpp
        math/f32/trunc.h
        io/implement/io.cpp
        io/io.h
        io/input_output/ops/implement/cio_out_ops.cpp
        io/input_output/ops/cio_out_ops.h
        math/int/implement/trailing_zeros.cpp
        math/int/trailing_zeros.h
        math/f32/implement/fabs.cpp
        math/f32/fabs.h
        io/input_output/read/implement/str_read.cpp
        io/input_output/read/str_read.h
        math/attribute/attribute.h
)

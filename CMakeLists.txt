cmake_minimum_required(VERSION 3.31)
project(math)

set(CMAKE_CXX_STANDARD 20)

add_executable(math main.cpp
        math/implement/f32/internal/f32.h
        math/implement/f32/implement/f32.cpp
        math/implement/char/char.h
        math/implement/char/implement/char.cpp
        math/implement/char/nan_helper.h
        math/implement/f32/implement/f32_functions.cpp
        math/implement/f32/internal/f32_functions.h
        math/implement/f32/implement/snan_err.cpp
        math/implement/f32/internal/snan_err.h
        io/input_output/write/implement/str_write.cpp
        io/input_output/write/str_write.h
        string/implement/to_string.cpp
        string/to_string.h
        string/implement/string.cpp
        string/string.h
        io/implement/io.cpp
        io/io.h
        io/input_output/ops/implement/cio_out_ops.cpp
        io/input_output/ops/cio_out_ops.h
        math/implement/int/implement/trailing_zeros.cpp
        math/implement/int/trailing_zeros.h
        math/implement/f32/implement/fabsf.cpp
        math/implement/f32/internal/fabsf.h
        io/input_output/read/implement/str_read.cpp
        io/input_output/read/str_read.h
        math/implement/attribute/attribute.h
        math/implement/f32/implement/truncf.cpp
        math/implement/f32/internal/truncf.h
        math/c_math.h
        math/internal/hf_math.h
        math/implement/f32/implement/fmaf.cpp
        math/implement/f32/internal/fmaf.h
        math/implement/f32/internal/numbers.h
        math/implement/f32/implement/force_eval.cpp
        math/implement/f32/internal/force_eval.h
        math/implement/f32/implement/nextafterf.cpp
        math/implement/f32/internal/nextafterf.h
        math/implement/f32/implement/frexpf.cpp
        math/implement/f32/internal/frexpf.h
        math/implement/f32/implement/scalbnf.cpp
        math/implement/f32/internal/scalbnf.h
        evil.cpp
        math/implement/f32/internal/round_mode.h
        math/implement/f32/implement/ldexpf.cpp
        math/implement/f32/internal/ldexpf.h
        math/implement/f32/internal/radix.h
)

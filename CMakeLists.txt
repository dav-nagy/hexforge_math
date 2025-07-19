cmake_minimum_required(VERSION 3.31)
project(math)

set(CMAKE_CXX_STANDARD 20)

# Add NASM object
enable_language(ASM_NASM)

# NASM x64
set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)

add_executable(math main.cpp
        math/implement/f32/f32.h
        math/implement/f32/implement/f32.cpp
        math/implement/char/char.h
        math/implement/char/implement/char.cpp
        math/implement/char/nan_helper.h
        math/implement/f32/implement/f32_util.cpp
        math/implement/f32/f32_util.h
        math/implement/f32/implement/snan_err.cpp
        math/implement/f32/snan_err.h
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
        math/implement/f32/fabsf.h
        io/input_output/read/implement/str_read.cpp
        io/input_output/read/str_read.h
        math/implement/attribute/attribute.h
        math/implement/f32/implement/truncf.cpp
        math/implement/f32/truncf.h
        math/c_math.h
        math/internal/hf_math.h
        math/implement/f32/implement/fmaf.cpp
        math/implement/f32/fmaf.h
        math/implement/f32/numbers.h
        math/implement/f32/implement/force_eval.cpp
        math/implement/f32/force_eval.h
        math/implement/f32/implement/nextafterf.cpp
        math/implement/f32/nextafterf.h
        math/implement/f32/implement/frexpf.cpp
        math/implement/f32/frexpf.h
        math/implement/f32/implement/scalbnf.cpp
        math/implement/f32/scalbnf.h
        math/implement/round_mode.h
        math/implement/f32/implement/ldexpf.cpp
        math/implement/f32/ldexpf.h
        math/implement/f32/radix.h
        math/implement/f32/implement/ilogbf.cpp
        math/implement/f32/ilogbf.h
        math/implement/f32/implement/copysignf.cpp
        math/implement/f32/copysignf.h
        main.cpp
        math/implement/f32/implement/modff.cpp
        math/implement/f32/modff.h
        math/implement/f32/implement/fmaxf.cpp
        math/implement/f32/fmaxf.h
        math/implement/f32/implement/fminf.cpp
        math/implement/f32/fminf.h
        math/implement/f32/implement/floorf.cpp
        math/implement/f32/floorf.h
        math/implement/f32/implement/ceilf.cpp
        math/implement/f32/ceilf.h
        math/implement/f32/implement/roundf.cpp
        math/implement/f32/roundf.h
        math/implement/f32/implement/rintf.cpp
        math/implement/f32/rintf.h
        math/implement/f32/implement/sqrtf.cpp
        math/implement/f32/sqrtf.h
        math/implement/f32/implement/cbrtf.cpp
        math/implement/f32/cbrtf.h
)

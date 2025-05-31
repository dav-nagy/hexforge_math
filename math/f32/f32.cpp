//
// Created by David on 5/30/2025.
//
//might use later_

#include "f32.h"

float inff() {
    return ieee754_f32(0x7f800000)._f;
}

float inff(const bool _n) {
    return ieee754_f32(0x7f800000 | (static_cast<int>(_n) << 31))._f;
}

float nanf(const char* _info) /*So this actually has practical applications in debugging later math functions!*/ {
    int _info_fits = reinterpret_cast<unsigned long long>(_info); // This feels shady but clang tidy doesn't seem to mind
    if constexpr (sizeof(_info) == 8 /*compile-time 64-bit pointer detection (I love this new if constexpr feature)*/)
        _info_fits >>= 42; // 64 - 22 = 42, should shift leading bit to be first in mantissa apart from quiet NaN
    else
        _info_fits >>= 10; // Same deal but with 32-bit pointers...
    return ieee754_f32(static_cast<int>(_info_fits) | 0x7fc00000 /*NaN exponent and leading mantissa bit*/)._f;
}
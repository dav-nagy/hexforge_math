//
// Created by David on 5/31/2025.
//

#include "../f32_functions.h"
#include "../f32.h"
#include "../../char/nan_helper.h"

float inff() {
    return ieee754_f32(0x7f800000)._f;
}

float inff(const bool _n) {
    return ieee754_f32(0x7f800000 | (static_cast<int>(_n) << 0x1f))._f;
}

float nanf(const char* _msg, const bool _quiet = true){
    //So this actually has practical applications in debugging later math functions!

    const int _s_msg_int = parse_nan_char(_msg);
    const unsigned int _msg_int = _s_msg_int * (_s_msg_int > 0 ? 1 : -1);
    const bool _sgn = (_s_msg_int < 0);
    ieee754_f32 _r((_msg_int| 0x7f800000) /*Infinite exponent, will determine leading mantissa bit later*/
        & ((_quiet << 0x16) | 0x7fbfffff));
    // 0x7fbfffff = 0b1111111101111111111111111111111 (Everything but sign and quiet nan flag)

    // Sign of input _msg is reflected in the sign bit
    _r._f_core._sgn = _sgn;
    return _r._f;
}

bool is_qnanf(const float _f) {
    return (ieee754_f32(_f)._i & 0x7fc00000) == 0x7fc00000;
}

bool is_snanf(const float _f) {
    return (ieee754_f32(_f)._i & 0x7fc00000) == 0x7f800000;
}
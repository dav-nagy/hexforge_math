//
// Created by David on 5/31/2025.
//

#include "../char.h"
#include "../nan_helper.h"

#define MAX_QUIET_NAN_INFO_32 0x3FFFFF // 22 bits (out of 23 - MSB = quiet NaN flag)

//Internal file for parsing the const char* argument in  nanf(...)
//Given a string of base-10 digits of type const char*, this function will return the integral value bounded to 2^22 (Maximum mantissa info)
int hexforge_nan_char::parse_nan_char_32(const char* _str) {
    if (_str == nullptr) return 0;
    int _r = 0;
    const int _sgn = (_str[0] == '-')? -1 : 1, _start = (_str[0] == '-')? 1 : 0;
    for (int _i = _start; _str[_i] != '\0'; _i++) {
        if (_str[_i] < '0' ||
            _str[_i] > '9' ||
            _r * 10 > (MAX_QUIET_NAN_INFO_32 - _str[_i] + '0')) return 0; // All these cases produce an invalid integer, so they are returned as 0
        // Avoiding division... ^This Czechs if the next iteration will exceed the 32-bit integer limit
        _r *= 10;
        _r += (_str[_i] - '0');
    }
    return _r * _sgn;
}

#undef MAX_QUIET_NAN_INFO

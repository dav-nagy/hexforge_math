//
// Created by David on 5/31/2025.
//

#include "../char.h"
#include "../nan_helper.h"

#define MAX_QUIET_NAN_INFO 0x3FFFFF // 22 bits (out of 23 - MSB = quiet NaN flag)

int hexforge_nan_char::parse_nan_char(const char* _str) {
    int _r = 0;
    const int _sgn = (_str[0] == '-')? -1 : 1, _start = (_str[0] == '-')? 1 : 0;
    for (int _i = _start; _str[_i] != '\0'; _i++) {
        if (_str[_i] < '0' ||
            _str[_i] > '9' ||
            _r * 10 > (MAX_QUIET_NAN_INFO - _str[_i] + '0')) return 0; // All these cases produce an invalid integer, so they are returned as 0
        // Avoiding division... ^This Czechs if the next iteration will exceed the 32-bit integer limit
        _r *= 10;
        _r += (_str[_i] - '0');
    }
    return _r * _sgn;
}

#undef MAX_QUIET_NAN_INFO

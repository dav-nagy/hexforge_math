//
// Created by David on 6/1/2025.
//

#include "../trailing_zeros.h"

#include "../../attribute/attribute.h"

//Return the number of trailing zeros in a binary representation of a 32-bit integer _i
//(e.g. trailing_zeros(32 (0b100000)) = 5
//This works with two's compliment negative numbers as well
extern "C"
_internal
int _int32_trailing_zeros(int _i) {
    _i = ~_i & (_i - 1); //Weird bit magic from the Java STL that I don't understand
    if (_i <= 0)
        return _i & 0x20;
    int _n = 1;
    if (_i > 0x10000) {_n += 0x10; _i >>= 0x10;}
    if (_i > 0x00100) {_n += 0x08; _i >>= 0x08;}
    if (_i > 0x00010) {_n += 0x04; _i >>= 0x04;}
    if (_i > 0x00004) {_n += 0x02; _i >>= 0x02;}
    return _n + (_i >> 1);
}

extern "C"
    _strong_alias(c_trailing_zeros, _int32_trailing_zeros);
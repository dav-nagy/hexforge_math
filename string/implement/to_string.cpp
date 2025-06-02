//
// Created by David on 6/1/2025.
//

#include "../to_string.h"

#include "../../math/f32/f32.h"
#include "../../math/f32/f32_functions.h"

using namespace hexforge_string;

//Convert a signed 32-bit integer _i to a base-10 string
//If any symbols save 0-9 and '-' are used, the function returs '0'
string hexforge_to_string::to_string(const int& _i) {
    unsigned int _ai = (_i < 0) ? -_i : _i;
    const bool _is_n = (_ai != _i);
    if (_i == 0)
        return string('0');
    char _buffer[12];
    int _index = 0;
    //Iterates through the number going from right to left, resulting in a reversed string
    while (_ai > 0) {
        _buffer[_index++] = '0' + (_ai % 10);
        _ai /= 10;
    }
    if (_is_n)
        _buffer[_index++] = '-';

    //Reverse the string
    int _start = 0, _end = _index - 1;
    while (_start < _end) {
        //Really cool XOR swap algorithm
        _buffer[_start] ^= _buffer[_end];
        _buffer[_end] ^= _buffer[_start];
        _buffer[_start] ^= _buffer[_end];
        _start++;
        _end--;
    }

    _buffer[_index] = '\0';

    return string(_buffer);
}

// using namespace hexforge_f32_functions;
//
// string to_string(const float& _f) {
//     if (is_nanf(_f)) return string("nan");
//     if (is_inff(_f)) return string("inf");
//     if (is_ninff(_f)) return string("-inf");
//
//     float _af = (_f < 0) ? -_f : _f;
//
//     char _buffer[32];
//     int _index = 0;
//
//     if (_af != _f)
//         _buffer[_index++] = '-';
//
// }

// string to_string::to_string(const float& _f) {
//
// }
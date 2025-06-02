//
// Created by David on 6/1/2025.
//

#include "../trunc.h"
#include "../f32.h"

float hexforge_trunc::truncf(float _f) {
    hexforge_f32::ieee754_f32 _fx(_f);
    unsigned int _sgn = _fx._f_core._sgn;
    // |_f|
    _fx._i &= 0x7fffffff;
    //If |_f| is greater than this, it's an integer, infinity, or NaN
    if(_fx._i >= 0x4b7fffff)
        return _f;
    const unsigned int _e = _fx._f_core._exp - 0x07f; // Subtract Exponent bias
    _fx._f_core._mantissa &= ((1 << _e + 1) - 1) << 0x017 /*23*/ - _e; // Mantissa bit mask
    return _fx._f;
}


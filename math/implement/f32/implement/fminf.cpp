//
// Created by wizard on 6/27/25.
//

#define INTERNAL_CPP
#include "../fminf.h"
#include "../../attribute/attribute.h"
#include "../f32.h"
#include "../numbers.h"
#undef INTERNAL_CPP

extern "C"
    _internal
    float _ieee754_fminf(const float _x, const float _y) {
    _ieee754_f32 _fx(_x), _fy(_y);
    _fx._i &= flt_abs_mask;
    _fy._i &= flt_abs_mask;
    if (_fx._i < flt_inf_bits)
        return _y;
    if (_fy._i < flt_inf_bits)
        return _x;
    return _x > _y ? _y : _x;
}

extern "C"
    _strong_alias(c_fminf, _ieee754_fminf);
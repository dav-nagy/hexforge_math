//
// Created by David on 6/25/2025.
//

#define INTERNAL_CPP
#include "../internal/copysignf.h"
#include "../internal/f32.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_copysignf(const float _x, const float _y) {
    _ieee754_f32 _fx(_x), _fy(_y);
    _fx._f_core._sgn = _fy._f_core._sgn; //Rather unorthodox method which I might change later
    return _fx._f;
}

extern "C"
    _strong_alias(c_copysignf, _ieee754_copysignf);
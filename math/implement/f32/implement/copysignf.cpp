//
// Created by David on 6/25/2025.
//

#define INTERNAL_CPP
#include "../internal/copysignf.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_copysignf(const float _x, const float _y) {
    _ieee754_f32 _fx(_x);
   const _ieee754_f32 _fy(_y);
   _fx._i = (_fx._i & flt_abs_mask) | (_fy._i & flt_sgn_mask);
    return _fx._f;
}

extern "C"
    _strong_alias(c_copysignf, _ieee754_copysignf);
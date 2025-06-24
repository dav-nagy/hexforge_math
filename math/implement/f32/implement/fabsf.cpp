//
// Created by David on 6/2/2025.
//

#define INTERNAL_CPP
#include "../internal/fabsf.h"
#include "../internal/f32.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C" //Hide from C++ via inline C to avoid name mangling
    _internal //Make the internal implementation invisible to declutter the public API
    float _ieee754_fabsf(const float _f) {
        _ieee754_f32 _fx(_f);
        _fx._i &= 0x7fffffff; // 32-bit floating point exp/mantissa mask (Discards sign bit)
        return _fx._f;
    }

extern "C" //This must go in extern "C" to avoid name mangling (Already defined in _strong_alias()
    //Format it nicely for the public API
    _strong_alias(c_fabsf, _ieee754_fabsf);
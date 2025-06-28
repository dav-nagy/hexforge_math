//
// Created by David on 6/13/2025.
//


#define INTERNAL_CPP
#include "../internal/frexpf.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_frexpf(const float _x, int* _exp) {
    _ieee754_f32 _ix(_x);
    //Sign bit of _x shorted 31 places so it is nicely lined up with the floating-point format
    const unsigned int _sgn = _ix._i & flt_sgn_mask;
    //|_x|
    _ix._i &= flt_abs_mask; //We need to do this for special case detections; the sign will be restored later
    *_exp = 0;
    //_x is +-inf/NaN/zero, we can return the value directly
    if (_ix._i >= flt_inf_bits || _ix._i == 0)
        return _x;
    if (_ix._f_core._exp == 0) { //Subnormal _x
        *_exp -= 25; //Scale down exponent because algebra
        _ix._f *= 0x1p25f; //Scale up _x so it is easier to work with
    }
    *_exp += _ix._f_core._exp - 126; //Generally the mantissa is between 1 and 2.
                                     //Right now we need it to be between 0.5 and 1,
                                     //so we divide by 2 by incrementing the exponent
    //TLDR; We need _ix._f e [0.5, 1.0)
    _ix._f_core._exp = 126; //Gets the value of _ix._f between 0.5 and 1
    _ix._i |= _sgn; //Restore the original sign
    return _ix._f;
}

extern "C"
    _strong_alias(c_frexpf, _ieee754_frexpf);
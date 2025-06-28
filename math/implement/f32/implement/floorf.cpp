//
// Created by wizard on 6/28/25.
//

#define INTERNAL_CPP
#include "../internal/floorf.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_floorf(const float _f) {
    _ieee754_f32 _fx(_f);
    const float _s = (_fx._f < 0.0f) ? 1.0f : 0.0f; //If negative, = 1, else 0
                                                    //(Intentionally misses signed zero)
    if (_fx._f_core._exp < flt_exp_bias) {//|_x| < 1.0f
        _fx._i &= flt_sgn_mask; //Get a signed zero
        return _fx._f - _s; //0.0f if positive, else -1.0f
    }
    //Why not just return -_s? Well, then we get a wrongly-signed zero.
    if (_fx._f_core._exp >= 150) //_fx._f is large enough to always be integral (>= 2^23)
        //This also catches inf/NaN
        return _f;
    const unsigned int _e = _fx._f_core._exp - flt_exp_bias; //Unbiased exp
    _fx._f_core._mantissa &= ((1 << (_e + 1)) - 1) << (flt_mant_len /*23*/ - _e); // See truncf() impl
    return _fx._f - (_s * (_fx._f != _f)); //_fx._f is the turncated _f. If _f was already integral, don't subtract 1
}

extern "C"
    _strong_alias(c_floorf, _ieee754_floorf);
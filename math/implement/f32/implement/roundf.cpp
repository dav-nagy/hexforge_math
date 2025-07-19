//
// Created by wizard on 6/29/25.
//

/*
 * 32-bit roundf() function _ieee754_roundf()
 *      Returns _f rounded to the nearest integer, tiing
 *      away from zero.
 *
 *  Method:
 *      roundf(f) = truncf(f + 0.5sgn(f))
 *      truncf logic is inlined
 *
 *      Until 2^23, where all floats are integral,
 *      adding 0.5f is always exact, due to the nature
 *      of 32-bit floating-point numbers.
 *
 *      At 2^23-region, and over, we can just return early,
 *      It is imperative that we do not do this check after adding
 *      +-0.5f, because in certain rounding modes, this will lead
 *      to an increase by 1 or more, which would return incorrect
 *      results.
 *
 */

#define INTERNAL_CPP
#include "../internal/roundf.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_roundf(const float _f){
    _ieee754_f32 _fx(_f);
    //|_f| is always integral/inf/NaN
    if (_fx._f_core._exp >= 150)
        return _f;
    const unsigned int _sgn = _fx._i & flt_sgn_mask;
    _fx._i &= flt_abs_mask;
    _fx._f += 0.5f; //Always exact now that integral cases have been ruled out
    _fx._i |= _sgn;
    //|_f +- 0.5f| > 1.0f,
    if (_fx._f_core._exp < flt_exp_bias) {
        _fx._i &= flt_sgn_mask; //Get a signed zero
        return _fx._f;
    }
    const unsigned int _e = _fx._f_core._exp - flt_exp_bias; // Subtract Exponent bias (0x07f = 127)
    _fx._f_core._mantissa &= ((1 << (_e + 1)) - 1) << (flt_mant_len /*23*/ - _e); // Mantissa bit mask as implemented above...
    return _fx._f;
}

extern "C"
    _strong_alias(c_roundf, _ieee754_roundf);

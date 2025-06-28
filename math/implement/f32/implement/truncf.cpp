//
// Created by David on 6/1/2025.

#define INTERNAL_CPP
#include "../internal/truncf.h"//These file have checks for INTERNAL_CPP, so we wrap these #includes in said macro
#include "../internal/f32.h"//...
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

/*
 * Basically, an integer represented as a float will never have bits past the
 * nth leading mantissa bit, where n is the unbiased exponent.
 * This lets us create a mask by getting all 1's in the
 * mantissa, then shifting it by 23 - n, because the
 * mantissa is 23 bits long.
 *
 * Demonstration:
 * 511.70f :
 *
 * 0 - 10000111 - 11111111101100110011010
 * ^      ^                  ^
 *sign   exp             mantissa
 * AND the mantissa with 11111111100000000000000
 * 0 - 10000111 - 11111111101100110011010
 * &              11111111100000000000000
 * We return 511.0f, which is this in binary:
 * 0 - 10000111 - 11111111100000000000000
 * The double implementation is similar to this, but the mantissa
 * is 53 bits wide, and there is a different exponent bias.
 */

///Return the smallest integral value smaller than a 32-bit floating point number _f (Rounding towards zero)
///@param _f 32-bit floating-point number to round towards zero
    extern "C"
    _internal // Some evil gatekeeping to keep the public API clean
    float _ieee754_truncf(const float _f) {
    _ieee754_f32 _fx(_f);
    //Detect subnormal input and all inputs below |+-1.0|
    //We round these all to +-zero, depending on the sign bit
    if (_fx._f_core._exp < flt_exp_bias) {
        _fx._i &= flt_sgn_mask; //We can do this because subnormal / below-1.0f floats trunc to zero. This leaves only the sign bit.
        return _fx._f;
    }
    //If |_f| is greater than this, it's an integer, infinity, or NaN
    if(_fx._f_core._exp >= 150)
        //Smallest number after 2^23-ulp (i.e. the largest decimal representable in 32 bits)
        return _f;
    const unsigned int _e = _fx._f_core._exp - flt_exp_bias; // Subtract Exponent bias (0x07f = 127)
    _fx._f_core._mantissa &= ((1 << (_e + 1)) - 1) << (flt_mant_len /*23*/ - _e); // Mantissa bit mask as implemented above...
    return _fx._f;
}

extern "C"//This must go in extern "C" to avoid name mangling
    //Format it nicely for the public API
    _strong_alias(c_truncf, _ieee754_truncf);
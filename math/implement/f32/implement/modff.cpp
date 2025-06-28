//
// Created by wizard on 6/27/25.
//

/*
 * 32-bit modff() function _ieee754_modff():
 *      Returns the fractional part of _x, and stores the integral part in *_iptr
 *
 * Method:
 *      We get rid of early returns first.
 *      If _x is +-zero or +-NaN, return that (including sign) and store it in *_iptr.
 *      If _x is +-inf, return 0 and store _x in *_iptr.
 *      For any _x below 1, we simply return and set *_iptr to +-zero.
 *      There is a point at 2^23 where fractional parts stop being representable
 *          in 32 bits. After that, everything is an integer, so we can just return
 *          zero and store _x in *_iptr.
 *
 * Then we move on to real-ish work:
 *      The Sterbenz Lemma says:
 *          If x and y are floating-point numbers, and if y/2 <= x <= 2y, then
 *          (x - y) is always exact.
 *      If we let _y be truncf(_x), then almost always this holds true. Even in
 *      1.(...) scenarios (The smallest where we do this), 0.5 <= _x <= 2.0, so
 *      it still works out. So, set *_iptr to truncf(_x), and return (_x - *_iptr)
 *          [Alternatively, call another turncf].
 *      Because of the Sterbenz Lemma, we can be confident that our result is
 *      always exact with on error of exactly 0ulp!
 *
 *      We integrate truncf() directly into this function to avoid function call
 *      overhead, but the premise is exactly the same. Therefore, it will look
 *      very simpilar to the truncf() implementation, so feel free to compare.
 */

#define INTERNAL_CPP
#include "../internal/modff.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_modff(const float _f, float * _iptr) {
    _ieee754_f32 _fx(_f);
    if (_fx._f_core._exp == 255) {
        //+-inf/NaN. We can do these checks before discarding the sign
        if (_fx._f_core._mantissa) { //NaN
            *_iptr = _fx._f; //Get the sign and make a signed NaN
            return _fx._f;
        }
        //+-inf
        *_iptr = _fx._f; //*_iptr holds the infinite part, while we return zero.
        _fx._i &= flt_sgn_mask; //Get a signed zero.
        return _fx._f;
    }
    const unsigned int _sgn = _fx._i & flt_sgn_mask;
    //All inputs less than one. This also covers zero
    if (_fx._f_core._exp < flt_exp_bias) {
        _fx._i = _sgn; //Create a signed zero
        *_iptr = _fx._f;
        return _f;
    }
    if (_fx._f_core._exp >= 23 + flt_exp_bias) { //After this number, all floats are integral.
        //This is the largest float with a fractional part.
        *_iptr = _fx._f; //The signed input goes in here
        _fx._i = _sgn; //Create a signed zero
        return _fx._f;
    }
    //All the special cases have now been accounted fore
    //Now we truncate the value and store it in *_iptr.
    const unsigned int _e = _fx._f_core._exp - flt_exp_bias; //Unbiased exponent
    //The mask below clears all the fractional bits from the mantissa with some neat math.
    _fx._f_core._mantissa &= ((1 << (_e + 1)) - 1) << (flt_mant_len - _e); //See truncf() impl.
    _fx._i |= _sgn;
    *_iptr = _fx._f; //*_iptr now holds the truncated value of _f.
    //Before we return _f - _fx._f, if _f was integral, subtracting these will create a zero.
    //However, it will be a positive zero! We must make sure that does not happen.
     _fx._f = _f - _fx._f; //Guaranteed exact via the Sterbenz Lemma.
     _fx._i |= _sgn; //Restore sign in possible zero
    return _fx._f;
}

extern "C"
    _strong_alias(c_modff, _ieee754_modff);
//
// Created by David on 6/13/2025.
//

/*
 * Method for subnormal results from normal numbers...
 * Multiply the normal number _fx by a power of 2 to get it subnormal
 * Record the multiplicand's exponent and subtract it from _e (Since _fx's exponent is zero)
 * Then, shift the subnormal's mantissa by _e (>>= -_e, since _e is always negative or zero)
 *
 * This would work except for one small thing...
 *      The mantissa gets truncated, not rounding to the nearest value.
 *      This means that in about half the cases, the result will be 1ulp
 *      too low, which the Knowers will not tolerate!
 *
 *      The solution is actually quite simple, however!
 *      We can just check for the lsb after shifting by _e - 1 (Toward zero)
 *      If this bit is set, then the final result has an error of >= 0.5ulp .
 *      Therefore, we can store this boolean in an integer _r.
 *      Then, we add _r to the final mantissa. If it is zero, nothing happens.
 *      But if it is one, i.e. we need to round up, it will increment the
 *      mantissa, doing exactly what we want!
 *
 *      This is what would work for the default rounding mode, to nearest.
 *      In ../internal/round_mode.h, there are some nice functions for
 *      determining the round mode. You can add checks, and do different
 *      rounding methods depending on the rounding mode. For instance,
 *      in _round_toward_zero, you just don't need to worry about _r
 *      at all, since truncation is intended.
 *
 * Return the result.
 * It should work???
 */

#define INTERNAL_CPP
#include "../internal/scalbnf.h"
#include "../internal/f32.h"
#include "../internal/f32_util.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"
#include "../internal/numbers.h"

extern "C"
    _internal
    float _ieee754_scalbnf(const float _x, const int _exp) {
    _ieee754_f32 _fx(_x);
    const unsigned int _sgn = _fx._i & _flt_sgn_mask; //Sign of the result which we add back to |_x| later
    _fx._i &= _flt_abs_mask; //Discard the sign bit so we can do checks easily
    int _e = _exp;
    if (_exp == 0 || _fx._i == 0 || _fx._i >= _flt_inf) //If _exp == 0 (We are multiplying by 1)
        //Or if |_x| is zero / inf / NaN, in all these cases we return _x early.
        return _x;

    const int _r_exp = _fx._f_core._exp + _e; //This is used for overflow/underflow checks
    //Use a modified version of the MUSL implementation
    //We have overflow/underflow checks to get rid of a bunch of nested if()'s
    if (_r_exp >= 255) {
        //We catch overflow early so we don't have to go into the crazy statements
        _fx._i = 0x7f800000 | _sgn;
        return _fx._f;
    }
    if (_r_exp < -23) { //The result is too small to store even as a subnormal!
        _fx._i = _sgn; //This will create a signed zero
        return _fx._f;
    }
    if (_e > 127) { //If _e is huge we should scale it down to be sure it fits in the new exponent
        _fx._f *= 0x1p127f;
        _e -= 127;
    } else if (_e < -126) { //Similarly, we scale it up if it is too small.
        _fx._f *= 0x1p-102f; // Equivalent to _fx._f *= 0x1p-126f * 0x1p24f;
        _e += 102; // Same deal, equivalent to _e += 126 - 24;
    }
    const float _s = _fx._f; //Store the nicely scaled version for multiplication
    _fx._i = ((0x7f + _e) << 23); //We reuse this union because it is cheap and easy.
                                  //This is just a power of 2 sith our new exponent inside _fx._f
    _fx._f *= _s;
    return _fx._f;

    /*
     * The following is considered deprecated, but I kept it for reference.
     * Be warned: It can stray from the exact result by up to one ulp for subnormal outputs
     * The _y and _z method is less accurate than I thought.
     */

    // if (!_fx._f_core._exp) {
    //     _fx._f *= 0x1p23f;
    //     _e -= 23;
    // }
    //
    // int _r_exp = _fx._f_core._exp + _e;
    //
    // if (_r_exp >= 255) {
    //     return c_ninff(_sgn);
    // }
    // if (_r_exp <= 0) { // The result is sybnormal
    //     if (_r_exp < -23 /*127 - 23*/) { //Too tiny to be represented even as a subnormal
    //         _fx._i = _sgn;   // +-0.0f
    //         return _fx._f;
    //     }
    //
    //     int _t = _fx._f_core._exp;
    //     _ieee754_f32 _y(0.0f), _z(1.0f);
    //     if (127 - _t < 0) {
    //         _y._f_core._exp = 0;
    //         _z._f_core._exp = -_t + 127;
    //     }
    //     else
    //         _y._f_core._exp = 127 - _t; //Equivalent to -(_fx._f_core._exp - 127)
    //
    //     //_y._f_core._exp = 127 - _t;
    //     //   This approach is NAÏVE and EVIL!
    //     //   On large enough _fx, This number will become negative
    //     //   and thus overflow. I must find a way of computing _y
    //     //   that accounts for negative decimals (i.e. subnormal _y)
    //
    //     /*
    //      * METHOD PROPOSAL FOR COMPUTING _y:
    //      *
    //      * Make a temporary variable _t to store the exponent of _y
    //      *
    //      * We know that 255 >= _t >= 0 (i.e. 128 >= unbiased _t >= -127)
    //      *
    //      * If _t is not less than zero, just use that as the exponent
    //      *
    //      * If not, set _y's exponent to zero.
    //      * Then, negate _t (This effectively gets its difference from 0)
    //      * Then, since 127 >= t >= 0 is always true, we can bias this with 127.
    //      * Now, we set another float _z's exponent to _t.
    //      * Now we have two numbers so that (_fx * _y) * _z will be subnormal!
    //      *            (Warning: computing _y * _z directly will underflow)
    //      * Now we just need to add both _y's and _z's exponents to _e
    //      *            (Actually, only _z's, since _y's will be zero),
    //      *  and everything should be fine!
    //      */
    //
    //     _fx._f *= _y._f;
    //     _fx._f *= _z._f;
    //     _e -= _y._f_core._exp - 127;
    //     _e -= _z._f_core._exp - 127;
    //
    //     //Note: This results in a correct result in _round_tonearest
    //     //There is also no tie functionality. The default mode is round-to-nearest, tie to even.
    //     //So, if there is a tie, but the fully-shifted result is even, we will get an invalid mantissa increment!
    //     int _round = (_fx._f_core._mantissa >> -(_e + 1)) & 1;
    //
    //     /*Method for ties:
    //      * A tie happens in a mantissa _m when bit -(_e + 1) is 1, and all less significant bits are zero.
    //      * So, if we shift by -(_e + 2) bits and back, we should not lose precision...
    //      * How can we detect if this is true?
    //      *      Well, we could directly shift, or we could do this:
    //      *      trailing_zeros(_m) == -(_e + 2)
    //      * If this is true, then we have a tie
    //      */
    //
    //     //I will find a nice way to implement the ties in the future
    //     //Right now I am far too lazy, and mathematically it only affects 1 in 2 ^ -(e + 1) cases... I think. That may just be completely wrong, but it looks right
    //     //Okay, that is actually a very big deal for _e close to zero...
    //
    //     _fx._f_core._mantissa >>= -_e; //Divide by power of 2
    //     _fx._f_core._mantissa += _round;
    //     _fx._i |= _sgn;
    //     return _fx._f;
    // }
    // _fx._i |= _sgn;
    // _fx._f_core._exp = 127;
    // _fx._f_core._exp = _r_exp; //+ 127;
    // return _fx._f;
}

extern "C"
    _strong_alias(c_scalbnf, _ieee754_scalbnf);
//
// Created by David on 6/13/2025.
//

/*
 * The Mantle by Agalloch:
 *  Such a good album! Something you would listen to
 *  all cozied up with hot chocolate looking out at the fjords...
 *  It has such jolly little tunes, and you get lost in the desolation
 *
 *  Also, Ulver's Bergtatt is very good...
 */

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
#include "../internal/ldexpf.h"
#include "../internal/f32.h"
#include "../internal/f32_functions.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"
#include "../internal/numbers.h"

extern "C"
    _internal_hidden
    float _ieee754_ldexpf(const float _x, const int _exp) {

    //TODO: FIX ldexpf (for certain subnormal outputs, ties, and rounding modes)
    //  (this is even worse than fmaf to implement because
    //  I can't steal from glibc because glibc's version is inaccurate!)
    //  LLVM also uses DyadicFloat, so I can't steal from them either...

    _ieee754_f32 _fx(_x);
    const unsigned int _sgn = _fx._i & _flt_sgn_mask;
    _fx._i &= _flt_abs_mask;

    int _e = _exp;

    _fx._i &= _flt_abs_mask;
    if (_exp == 0 || _fx._i == 0 || _fx._i >= _flt_inf) {
        return _x;
    }

    if (!_fx._f_core._exp) {
        _fx._f *= 0x1p23f;
        _e -= 23;
    }

    int _r_exp = _fx._f_core._exp + _e;

    if (_r_exp >= 255) {
        return c_ninff(_sgn);
    }
    if (_r_exp <= 0) { // The result is sybnormal
        if (_r_exp < -23 /*127 - 23*/) { //Too tiny to be represented even as a subnormal
            _fx._i = _sgn;   // +-0.0f
            return _fx._f;
        }

        _ieee754_f32 _y(0.0f);

        _y._f_core._exp = 127 - _fx._f_core._exp; //Equivalent to -(_fx._f_core._exp - 127)
        //TODO: Right now this approach is NAÏVE and EVIL!
        //   On large enough _fx, This number will become negative
        //   and thus overflow. I must find a way of computing _y
        //   that accounts for negative decimals (i.e. subnormal _y)

        /*
         * METHOD PROPOSAL FOR COMPUTING _y:
         *
         * Make a temporary variable _t to store the exponent of _y
         *
         * We know that 255 >= _t >= 0 (i.e. 128 >= unbiased _t >= -127)
         *
         * If _t is not less than zero, just use that as the exponent
         *
         * If not, set _y's exponent to zero.
         * Then, negate _t (This effectively gets its difference from 0)
         * Then, since 127 >= t >= 0 is always true, we can bias this with 127.
         * Now, we set another float _z's exponent to _t.
         * Now we have two numbers so that (_fx * _y) * _z will be subnormal!
         *            (Warning: computing _y * _z directly will underflow)
         * Now we just need to add both _y's and _z's exponents to _e
         *            (Actually, only _z's, since _y's will be zero),
         *  and everything should be fine!
         */

        _fx._f *= _y._f;
        _e -= _y._f_core._exp - 127;

        //Note: This results in a correct result in _round_tonearest
        //There is also no tie functionality. The default mode is round-to-nearest, tie to even.
        //So, if there is a tie, but the fully-shifted result is even, we will get an invalid mantissa increment!
        int _round = (_fx._f_core._mantissa >> -(_e + 1)) & 1;

        //I will find a nice way to implement the ties in the future
        //Right now I am far too lazy, and mathematically it only affects 1 in 2^-e cases...
        //Okay, that is actually a very big deal for _e close to zero...

        _fx._f_core._mantissa >>= -_e; //Divide by power of 2
        _fx._f_core._mantissa += _round;
        _fx._i |= _sgn;
        return _fx._f;
    }
    _fx._i |= _sgn;
    _fx._f_core._exp = 127;
    _fx._f_core._exp = _r_exp; //+ 127;
    return _fx._f;
}

extern "C"
    _strong_alias(c_ldexpf, _ieee754_ldexpf);
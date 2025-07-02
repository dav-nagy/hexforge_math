//
// Created by wizard on 6/30/25.
//

//LLVM-type approach to sqrt() function using shift-subtract
//  LLVM impl: https://github.com/llvm/llvm-project/blob/main/libc/src/__support/FPUtil/generic/sqrt.h
/*
 *32-bit sqrtf() function _ieee754_sqrtf()
 *      Returns the correctly-rounded square root of x.
 *
 * It uses LLVM's shift-subtract method.
 * But unlike LLVM's DyadicFloat, which rounds for you, as well as
 * their use of a clz() asm intrinsic, this version does everything
 * dependency and architecture-free, except for little-endian assumptions.
 *
 * We can detect the rounding mode with the ../../round_mode.h file, which
 * has dependency-free rounding mode detection, not even <fenv>. (yayy!)
 *
 * Method:
 *      We handle special cases, which are trivial. (+-NaN, +inf, 0, -|x|)
 *
 *      For subnormal inputs, we scale up by 2^23, so even the smallest
 *      subnormals become normal. Then, we grab the mantissa for the sqrt
 *      approximation.
 *      For normal numbers, we just take the mantissa as-is.
 *
 *      We know that for a float _f, _f = 1.mm * 2^e (For normal floats).
 *      Therefore, sqrt(_f) = sqrt(1.mm) * 2^(e/2). So, we actually just
 *      need to find sqrt(1.mm). This is why we found the normalized mantissa.
 *
 *      If e is odd, we shift over the mantissa, and decrement e to make it even, so
 *      we can calculate e/2 (i.e. e << 1) exactly.
 *
 *      We convert the mantissa to fixed point by adding the implicit one bit (one_mant),
 *      and doing a shift-subtract approach to get the correctly rounded result.
 *      We actually calculate 2 more bits, one for the half-ulp (tie), and the last one is
 *      a sticky bit (For if any bits in the infinite bit-expansion after that are on). This
 *      lets us get correct rounding all the time.
 *
 *      This is all we need for rounding, actually. We use these 2 bits to turn our 25-bit
 *      sqrt into a 23-bit sqrt depending on the rounding mode.
 */

#define INTERNAL_CPP
#include "../internal/sqrtf.h"

#include "../../attribute/attribute.h"
#include "../../round_mode.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

static constexpr unsigned int one_mant =  (1 << 23);

extern "C"
    _internal
    float _ieee754_sqrtf(const float _f) {
    _ieee754_f32 _fx(_f);
    /*
     * sqrt(-inf) = NaN
     * sqrt(-|x|) = NaN
     */
    if (_fx._i > flt_sgn_mask) { //Not >=, because > discards signed zero, which we like.
        //flt_sgn_mask is a binary of only the sign bit, so any _f larger than that means _f < -0.0f
        _fx._i = 0x7f800001; //sNaN specifically (Quiet bit not set)
        return _fx._f;
    }
    /*
     * sqrt(+inf) = +inf
     * sqrt(+-NaN) = +-NaN
     * sqrt(+-0) = +-0
     * -inf and -x have been accounted for already
     */
    if (_fx._f_core._exp == 255 || _fx._f == 0.0f /*Thankfully holds true for -0.0f also*/)
        return _f;
    unsigned int _f_mant = _fx._f_core._mantissa,
                 _f_exp = _fx._f_core._exp - 127;
    //unsigned int subnorm = 0;
    if (!(_fx._f_core._exp)) { //_f is subnormal:
        _fx._f *= 0x1p23f; //Multiplication by a power of 2 is always exact in IEEE-754
        _f_mant = _fx._f_core._mantissa; //Now, it is a normalized mantissa
        _f_exp = _fx._f_core._exp - 150; // - 127 - 23
    }
    _f_mant |= one_mant;
    //Make sure the exponent is even.
    if (_f_exp & 1) {
        --_f_exp;
        _f_mant <<= 1;
    }
    //Now we move on to the fun part:
    //Magic shift-subtract sqrt method that I don't really understand that well.
    unsigned int _y = one_mant,
    _r = _f_mant - one_mant, _t;
    for (unsigned int _c_bit = (one_mant >> 1); _c_bit; _c_bit >>= 1) {
        _r <<= 1;
        //2 * _y(n - 1) + 2^(-n - 1)
        _t = ((_y << 1) + _c_bit);
        if (_r >= _t) {
            _r -= _t;
            _y += _c_bit;
        }
    }
    //Compute one more iteration on order to round correctly
    _r <<= 2;
    _y <<= 2;
    _t = _y + 1;
    if (_r >= _t) {
        _r -= _t;
        _y |= 2;
    }
    //Sticky bit
    _y |= (_r != 0);
    /*
     * We have a 25-bit mantissa which we need to get to 23 bits with rounding
     * However, FPU rounding can't help us in this case, so we need to implement
     * it ourselves depending on the current rounding mode.
     */
    unsigned int _round;
    //Decide what to do with the two extra bits based on the current rounding mode
    switch (c_get_f_round()) {
        case _round_up:
             // Always round the result up if it isn't exact. (sqrtf many times isn't)
            _round = (_y & 3) != 0; //1 if any of the round/sticky bits are set, else 0
            _y >>= 2;
            _y += _round;
            break;
        case _round_nearest:
            /*
             * Round to the nearest representable value, unless we are exactly 0.5ulp off (tie).
             * If so, we round to the value where the mantissa is even.
             */
            if ((_y & 3) == 2){ //Tie, round bit set but sticky isn't
                //We must tie-to-even for IEEE754 compliance.
                const bool _lsb = (_y >> 2) & 1; //Shifted mantissa lsb is odd
                _y >>= 2;
                 _y += _lsb; //Set lsb to zero
            } else { //No tie - we just round to nearest
                _round = (_y & 2) != 0; // 1 if we need to round up (i.e. round is set, sticky isn't).
                //If sticky is set, and round isn't, we round down to get to the nearest value.
                _y >>= 2;
                _y += _round;
            }
            break;
        default: //_round_zero and _round_down
            //These both function the same, since we are dealing with positive numbers only.
            _y >>= 2; //Simple truncation emulates this perfectly
            break;
    }
    /*
     * An alternate method would be to store this extended-precision mantissa in a double _z,
     * and the return static_cast<float>(_z);. However, this requires an extended precision format,
     * and it is safer to do it manually. Also, the double sqrt would not be able to use an extended
     * precision (Long double is implementation-defined).
     */
    //Return the actual sqrt:
    _fx._f_core._exp = 127 + (_f_exp >> 1); //Our halfed exponent
    _fx._f_core._mantissa = _y; //Our nice approximation for the 1.xx sqrtf()
    return _fx._f;
}

_strong_alias(c_sqrtf, _ieee754_sqrtf);
//
// Created by David on 6/25/2025.
//

#define INTERNAL_CPP
#include "../internal/ilogbf.h"
#include "../internal/f32.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

/*
 *For nan/zero arguments, it is implementation-defined.
 *We use the largest integers because they are closest
 *to the "true" results (As _f approaches these values)
 */

#define _ilogb_nan 2147483647 //INT_MAX
#define _ilogb_inf 2147483647 //INT_MAX
#define _ilogb_zero (-2147483648) //-INT_MAX

extern "C"
    _internal
    int _ieee754_ilogbf(const float _f) {
    _ieee754_f32 _fx(_f);
    _fx._i &= flt_abs_mask; //Absolute value for zero check
    if (_fx._f_core._exp == 255) {//The input is inf/NaN
        return _fx._f_core._sgn ? -_ilogb_inf : _ilogb_inf; //Same as _ilogb_nan. No need for extra ternary operator
    }
    if (_fx._i == 0) // If the input is zero, return _ilogb_zero
        return _ilogb_zero;
    if (_fx._f_core._exp) //Normal number with well-defined exponent
        return _fx._f_core._exp - 127; //Subtract bias
    //If _x is subnormal, we continue. All previous cases have been accounted for.
    _fx._f *= 0x1p23f; //We multiply by a big number to make sure that it becomes normal.
                       //Then, we just subtract 23 later
    return _fx._f_core._exp - 150; //- 127 - 23
}

#undef _ilogb_nan
#undef _ilogb_inf
#undef _ilogb_zero

extern "C"
    _strong_alias(c_ilogbf, _ieee754_ilogbf);
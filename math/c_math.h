//
// Created by David on 6/3/2025.
//



/*TODO:
 * ilogbf
 * copysignf
 * isfinitef
 * isnormalf
 * signbitf
 * modff
 * roundf
 * floorf
 * ceilf
 * nearbyintf
 * fpclassifyf
 */

#ifndef C_MATH_H
//Include Guard
#define C_MATH_H

#define C_MATH_INCLUDE

#include "internal/hf_math.h"
#include "implement/f32/internal/radix.h"

#ifdef NO_INCLUDE
    #error Something was included that should not have been.
#endif

namespace hf_math {
    using ::c_fabsf;

    ///Return the absolute value of _f (i.e. |_f|).
    ///
    ///For implementation, see implement/f32/implement/fabsf.cpp.
    ///@param _f The number to take the absolute value of.
    inline float fabs(const float _f)
    { return c_fabsf(_f); }

    using ::c_fmaf;

    ///Return _x * _y + _z as a ternary operation with one instance of rounding.
    ///
    ///For implementation, see implement/f32/implement/fmaf.cpp.
    ///@param _x One of the multiplicands, to be multiplied with _y.
    ///@param _y Another multiplicand, to be multiplied with _x.
    ///@param _z The independent summand, the other one being the product of _x and _y.
    inline float fma(const float _x, const float _y, const float _z)
    { return c_fmaf(_x, _y, _z); }

    using ::c_frexpf;

    ///Return a value _x so that _x ∈ [0.5, 1.0), and an integer _e
    ///such that _x * 2^_e = _f.
    ///
    ///For implementation, see implement/f32/implement/frexpf.cpp.
    ///@param _f The floating-point value to do the calculation on.
    ///@param _exp The integral exponent variable.
    ///(It is passed as an int pointer).
    inline float frexp(const float _f, int* _exp)
    { return c_frexpf(_f, _exp); }

    using ::c_ldexpf;

    ///Return the value _f * (radix)^_exp correctly rounded.
    ///On a system where the radix is 2 (i.e. a binary system), this is equivalent to scalbnf(_f, _exp).
    ///
    ///Only a radix of 2 is supported. If your system is not radix-2, that is exceedingly rare.
    ///
    ///For implementation, see implement/f32/implement/scalbnf.cpp
    ///or implement/f32/implement/ldexpf.cpp.
    ///@param _f The floating-point multiplicand.
    ///@param _exp The exponent of 2 to multiply _f by.
    inline float ldexp(const float _f, const int _exp)
        { return c_ldexpf(_f, _exp); }

    using ::c_scalbnf;

    ///Return the value _f * 2^_exp correctly rounded.
    ///
    ///For implementation, see implement/f32/implement/scalbnf.cpp
    ///or implement/f32/implement/ldexpf.cpp.
    ///@param _f The floating-point multiplicand.
    ///@param _exp The exponent of 2 to multiply _f by.
    inline float scalbn(const float _f, const int _exp)
        { return c_scalbnf(_f, _exp); }

    using ::c_inff;



    using ::c_ninff;

    using ::c_is_inff;
    using ::c_is_ninff;
    using ::c_is_pinff;

    using ::c_nanf;

    using ::c_is_nanf;
    using ::c_is_qnanf;
    using ::c_is_snanf;

    using ::c_nextafterf;

    using ::c_truncf;


    ///Return a 32-bit positive infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    inline float inf()
        { return c_inff(); }
    ///Return a 32-bit  infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///
    ///If the boolean is true, the infinity is negative.
    ///Otherwise, it is positive.
    ///@param _n Whether the infinity is negative or positive.
    ///@returns +-inf, depending on _n
    inline float inf(const bool _n)
        { return c_ninff(_n); }
    ///Detect if a given 32-bit value is infinity (either sign).
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for +-inf.
    inline bool is_inf(const float _f)
        { return c_is_inff(_f); }
    ///Detect if a given 32-bit value is negative infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for -inf.
    inline bool is_n_inf(const float _f)
        { return c_is_ninff(_f); }
    ///Detect if a given 32-bit value is positive infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for +inf.
    inline bool is_p_inf(const float _f)
        { return c_is_pinff(_f); }

    ///Return a 32-bit NaN (Not a Number).
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///
    ///The message is a base-10 integer recorded in the mantissa of the nan.
    ///The boolean is to signify whether the nan is quiet or signaling.
    ///@param _s The message to be encoded in the nan
    ///@param _q Whether the nan is quiet or signaling
    inline float nan(const char* _s, const bool _q)
        { return c_nanf(_s, _q); }

    ///Detect if a given 32-bit value is NaN (Not a Number, any type).
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for nan.
    inline bool is_nan(const float _f)
        { return c_is_nanf(_f); }
    ///Detect if a given 32-bit value is qNaN (Quiet NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for qnan.
    inline bool is_qnan(const float _f)
        { return c_is_qnanf(_f); }
    ///Detect if a given 32-bit value is sNaN (Signaling NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_functions.cpp.
    ///@param _f The value to be tested for snan.
    inline bool is_snan(const float _f)
        { return c_is_snanf(_f); }

    ///Compute the next 32-bit floating-point value after _x in the direction of _y.
    ///
    ///For implementation, see implement/f32/implement/nextafterf.cpp.
    ///@param _x The starting value, with the returned value being adjacent to this.
    ///@param _y The direction in which the return value will be adjacent to _x.
    ///(*Direction as seen on a number line).
    inline float nextafter(const float _x, const float _y)
        { return c_nextafterf(_x, _y); }

    ///Return the largest integral value that is smaller in magnitude than _f.
    ///(i.e. round _f toward zero)
    ///
    //////For implementation, see implement/f32/implement/truncf.cpp.
    ///@param _f The value to be truncated
    inline float trunc(const float _f)
        { return c_truncf(_f); }
}

#undef C_MATH_INCLUDE

#endif //C_MATH_H
//
// Created by David on 6/3/2025.
//

/*TODO:
 * modff
 * roundf
 * floorf
 * ceilf
 * nearbyintf
 */

#ifndef C_MATH_H
//Include Guard
#define C_MATH_H

#if !(defined(__GNUC__) || defined(__clang__))
    #error "This library runs only on compilers that respect the ancient ways (gcc/clang)"
#endif

#define C_MATH_INCLUDE

#include "implement/f32/internal/radix.h"

#ifdef NO_INCLUDE
    #error Something was included that should not have been.
#endif

extern "C"{
    float c_fabsf(float);

    float c_fmaf(float, float, float);

    float c_frexpf(float, int*);

    int c_ilogbf(float);

    float c_ldexpf(float, int);

    float c_scalbnf(float, int);

    float c_inff();
    float c_ninff(bool);

    bool c_is_inff(float);
    bool c_is_ninff(float);
    bool c_is_pinff(float);

    bool c_is_finitef(float);
    bool c_is_normalf(float);
    int c_signbitf(float);

    int c_fpclassifyf(float);

    bool c_is_nanf(float);
    bool c_is_qnanf(float);
    bool c_is_snanf(float);

    float c_nextafterf(float, float);

    float c_nanf(const char*, bool);

    float c_truncf(float);
}

/*
 * These values are the same as the ones in <cmath> / <math.h> because it is already standard.
 */

#define _fpclass_nan 0x0100
#define _fpclass_normal 0x0400
#define _fpclass_infinite (_fpclass_nan | _fpclass_normal)
#define _fpclass_zero 0x4000
#define _fpclass_subnormal (_fpclass_normal | _fpclass_zero)

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

    using ::c_ilogbf;

    ///Return the integral unbiased exponent of _f, ignoring the sign of _f.
    ///
    ///Special cases:
    ///
    ///If _f is zero, return -INT_MAX.
    ///If _f is inf/NaN, return INT_MAX.
    ///
    ///For implementation, see implement/f32/implement/ilogbf.cpp
    ///@param _f The floating-point value to take the integral logarithm of.
    inline int ilogb(const float _f)
    { return c_ilogbf(_f); }


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
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    inline float inf()
        { return c_inff(); }
    ///Return a 32-bit  infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///
    ///If the boolean is true, the infinity is negative.
    ///Otherwise, it is positive.
    ///@param _n Whether the infinity is negative or positive.
    ///@returns +-inf, depending on _n
    inline float inf(const bool _n)
        { return c_ninff(_n); }
    ///Detect if a given 32-bit value is infinity (either sign).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for +-inf.
    inline bool is_inf(const float _f)
        { return c_is_inff(_f); }
    ///Detect if a given 32-bit value is negative infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for -inf.
    inline bool is_n_inf(const float _f)
        { return c_is_ninff(_f); }
    ///Detect if a given 32-bit value is positive infinity.
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for +inf.
    inline bool is_p_inf(const float _f)
        { return c_is_pinff(_f); }

    ///Return a 32-bit NaN (Not a Number).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///
    ///The message is a base-10 integer recorded in the mantissa of the nan.
    ///The boolean is to signify whether the nan is quiet or signaling.
    ///@param _s The message to be encoded in the nan
    ///@param _q Whether the nan is quiet or signaling
    inline float nan(const char* _s, const bool _q)
        { return c_nanf(_s, _q); }

    ///Detect if a given 32-bit value is NaN (Not a Number, any type).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for nan.
    inline bool is_nan(const float _f)
        { return c_is_nanf(_f); }
    ///Detect if a given 32-bit value is qNaN (Quiet NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for qnan.
    inline bool is_qnan(const float _f)
        { return c_is_qnanf(_f); }
    ///Detect if a given 32-bit value is sNaN (Signaling NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for snan.
    inline bool is_snan(const float _f)
        { return c_is_snanf(_f); }

    ///Detect if a given 32-bit value is finite (not infinite or NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for finiteness
    inline bool is_finite(const float _f)
        { return c_is_finitef(_f); }

    ///Detect if a given 32-bit value is normal (Not zero, subnormal, infinite, or NaN).
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to be tested for normalcy
    inline bool is_normal(const float _f)
        { return c_is_normalf(_f); }

    ///Return the sign bit of a 32-bit value _f.
    ///If _f is positive, the sign bit is zero. Otherwise, it is negative.
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to extract the sign bit of
    inline int signbit(const float _f)
        { return c_signbitf(_f); }

    ///Return a classification value for a 32-bit value _f.
    ///
    ///@note The return values do indeed match those of <math.h> and <cmath>.
    ///
    ///For implementation, see implement/f32/implement/f32_util.cpp.
    ///@param _f The value to classify
    ///@returns 1: _fp_nan, 2: _fp_normal, 3: _fp_infinite, 4: _fp_zero, 5: _fp_subnormal
    inline int fpclassify(const float _f)
        { return c_fpclassifyf(_f); }

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
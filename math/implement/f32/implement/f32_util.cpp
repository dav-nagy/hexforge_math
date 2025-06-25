//
// Created by David on 5/31/2025.
//


#define INTERNAL_CPP
#include "../../char/nan_helper.h"
#include "../internal/f32.h"
#include "../internal/f32_util.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"
#include "../internal/numbers.h"

// Generate a 32-bit positive infinity
extern "C"
    _internal
    float _ieee754_inff() {
    return _ieee754_f32(0x7f800000)._f;
    }
//Generate a 32-bit infinity
//If _n is true, it returns negative infinity. If it is false, it returns infinity
extern "C"
    _internal
    float _ieee754_ninff(const bool _n = false) {
    return _ieee754_f32(0x7f800000 | (_n << 0x1f))._f;
}


//Detect if a 32-bit floating point number _f is infinity
extern "C"
    _internal
    bool _ieee754_is_pinff(const float _f) {
    return _ieee754_f32(_f)._i == 0x7f800000; // Bitfield for +inf
}
//Detect if a 32-bit floating point number _f is -infinity
extern "C"
    _internal
    bool _ieee754_is_ninff(const float _f) {
    return _ieee754_f32(_f)._i == 0xff800000; // Bitfield for -inf
}
//Detect if a 32-bit floating point number _f is any infinity
extern "C"
    _internal
    bool _ieee754_is_inff(const float _f) {
    return (_ieee754_f32(_f)._i & 0x7fffffff) /*|_f|*/ == 0x7f800000; //Bitfield for +inf
}

//Create a 32-bit NaN (Not a Number)
// _msg is to be a base-10 <=23-bit integer that will be encoded in the mantissa
// If _quiet is true, the function will generate a quiet NaN (i.e. a NaN that will propagate through functions)
// If _quiet is false, the function will generate a signaling NaN (i.e. a NaN that will throw an error if caught)
extern "C"
    _internal
    float _ieee754_nanf(const char* _msg, const bool _quiet = true){
    //So this actually has practical applications in debugging later math functions!

    const int _s_msg_int = hexforge_nan_char::parse_nan_char_32(_msg); //Converts the character message into an unsigned integer
    const unsigned int _msg_int = _s_msg_int > 0 ? _s_msg_int : -_s_msg_int; //Absolute value of the integer message (could be optimized!)
    const bool _sgn = (_s_msg_int < 0); //Sign bit of the integer message
    _ieee754_f32 _r((_msg_int| 0x7f800000) /*Infinite exponent mask, will determine leading mantissa bit later*/
                                 & ((_quiet << 0x16) | 0x7fbfffff));
    // 0x7fbfffff = 0b1111111101111111111111111111111 (Everything but sign and quiet nan flag)

    // Sign of input _msg is reflected in the sign bit
    _r._f_core._sgn = _sgn;
    return _r._f;
}

//Detect if a 32-bit floating point number _f is a quiet NaN
extern "C"
    _internal
    bool _ieee754_is_qnanf(const float _f) {
    return (_ieee754_f32(_f)._i & 0x7fc00000) == 0x7fc00000;
}
//Detect if a 32-bit floating point number _f is a signaling NaN
extern "C"
    _internal
    bool _ieee754_is_snanf(const float _f) {
    return (_ieee754_f32(_f)._i & 0x7fc00000) == 0x7f800000;
}
//Detect if a 32-bit floating point number _f is any type of NaN (Not a Number)
extern "C"
    _internal
    bool _ieee754_is_nanf(const float _f) {
    const _ieee754_f32 _fx(_f);
    return (_fx._f_core._exp == 0xff && _fx._f_core._mantissa != 0);
}

extern "C"
    _internal
    bool _ieee754_is_finitef(const float _f) {
    const _ieee754_f32 _fx(_f);
    return (_fx._f_core._exp < 255); //Only with an exponent of 255 is inf/nan
}

extern "C"
    _internal
    bool _ieee754_is_normalf(const float _f) {
    const _ieee754_f32 _fx(_f);
    return (_fx._f_core._exp < 255 && _fx._f_core._exp > 0);
}

extern "C"
    _internal
    int _ieee754_signbitf(const float _f) {
    const _ieee754_f32 _fx(_f);
    return _fx._f_core._sgn;
}

/*
 * Return values for fpclassifyf.
 * These are arbitrary.
 */
#define _fp_nan 0x0100
#define _fp_normal 0x0200
#define _fp_infinite (_fp_nan | _fp_normal)
#define _fp_zero 0x4000
#define _fp_subnormal (_fp_normal | _fp_zero)

extern "C"
    _internal
    int _ieee754_fpclassifyf(const float _f) {
    _ieee754_f32 _fx(_f);
    _fx._i &= _flt_abs_mask;
    if (_fx._f_core._exp == 0)
        return _fx._f_core._mantissa ? _fp_subnormal : _fp_zero;
    if (_fx._f_core._exp == 255)
        return _fx._f_core._mantissa ? _fp_nan : _fp_infinite;
    return _fp_normal;
}

//Get rid of some macros defiend for the fpclassify function
#undef _fp_nan
#undef _fp_normal
#undef _fp_infinite
#undef _fp_zero
#undef _fp_subnormal

//A bunch of _strong_aliases to display this to the .h file for inline wrappers / public API

extern "C"{
    _strong_alias(c_inff, _ieee754_inff);
    _strong_alias(c_ninff, _ieee754_ninff);

    _strong_alias(c_is_pinfff, _ieee754_is_pinff);
    _strong_alias(c_is_ninff, _ieee754_is_ninff);
    _strong_alias(c_is_inff, _ieee754_is_inff);

    _strong_alias(c_nanf, _ieee754_nanf);

    _strong_alias(c_is_qnanf, _ieee754_is_qnanf);
    _strong_alias(c_is_snanf, _ieee754_is_snanf);
    _strong_alias(c_is_nanf, _ieee754_is_nanf);

    _strong_alias(c_is_finitef, _ieee754_is_finitef);
    _strong_alias(c_is_normalf, _ieee754_is_normalf);
    _strong_alias(c_signbitf, _ieee754_signbitf);

    _strong_alias(c_fpclassifyf, _ieee754_fpclassifyf);

}
//
// Created by David on 5/30/2025.
//

#ifndef F32_H
//Include guard
#define F32_H

/*
 *Assuming little endian with 32 bit ints because frankly
 *I am too lazy not to and the only people who will be using
 *this are me and my friend anyway, who are on 64-bit Windows with g++/clang
 */

namespace hexforge_f32 {
    //IEEE754 32-bit Floating Point form
    union ieee754_f32 {
        float _f;
        unsigned int _i; //Might change to 'unsigned long _i : 32;' later...
        //'long' is guaranteed to be at least 32 bits; 'int', however, does not promise this (>= 16 bits)

        //Access the mantissa, exponent, and sign bit (i.e. the "core") of the float
        struct {
            //Mantissa of the floating point bitfield of _f (i.e. the fractional part, with an implicit 1 unless _f is subnormal)
            unsigned int _mantissa : 23;
            //Exponent of the floating point bitfield of _f (i.e. the power of 2 used to multiply the mantissa, -127 (bias))
            unsigned int _exp       : 8;
            //Sign bit of the floating point bitfield of _f (If _f is negative, this value is 1. Otherwise, it is 0)
            unsigned int _sgn       : 1;
        } _f_core;

        //Initialize ieee754 type with a floating point _f
        explicit constexpr ieee754_f32(const float _f) : _f(_f) {};
        //Initialize ieee754 type with an integer _i
        explicit constexpr ieee754_f32(const int _i) : _i(_i) {};
        //Initialize ieee754 type with an unsigned integer _i
        explicit constexpr ieee754_f32(const unsigned int _i) : _i(_i) {};
    };
}

#endif //F32_H

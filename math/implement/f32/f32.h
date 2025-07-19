//
// Created by David on 5/30/2025.
//

#ifndef F32_H
//Include guard
#define F32_H

/*
 *Assuming little endian with 32 bit ints because frankly
 *I am too lazy not to and the only people who will be using
 *this are me and my friend anyway, who are on 64-bit win/linux with g++/clang
 */

extern "C"
//IEEE754 32-bit Floating Point form
union _ieee754_f32 {
    /*Technically type punning like this is undefined behaviour as of C++23
     * However, no one actually cares because it works anyway (Even the linux kernel does it)
     * If you know the rules then you can responsibly break them
     * (It is defined on basically all compilers, i.e. the de facto standard)
     */

    ///Floating-point representation of the number.
    float _f;
    ///Integral bit representation of the number.
    unsigned int _i; //Might change to 'unsigned long _i : 32;' later...
    //'long' is guaranteed to be at least 32 bits; 'int', however, does not promise this (>= 16 bits)

    ///Access the mantissa, exponent, and sign bit (i.e. the "core") of the float.
    struct {
        ///Mantissa of the floating point bitfield of _f (i.e. the fractional part, with an implicit 1 unless _f is subnormal).
        unsigned int _mantissa : 23;
        ///Exponent of the floating point bitfield of _f (i.e. the power of 2 used to multiply the mantissa, -127 (bias)).
        unsigned int _exp       : 8;
        ///Sign of the floating point bitfield of _f (If _f is negative, this value is 1. Otherwise, it is 0).
        unsigned int _sgn       : 1;

    } _f_core;
    ///Access the high and low halves of the float (Sometimes useful, more used in double precision).
    struct {
        unsigned int
            ///Lower word of _f (i.e. the lower 16 bits).
            _low  : 16,
            ///Higher word of _f (i.e. the higher 16 bits).
            _high : 16;
    } _lh;

    ///Initialize ieee754 type with a floating point _f.
    ///@param _f The number to initialize the union via.
    explicit constexpr _ieee754_f32(const float _f) : _f(_f) {};
    ///Initialize ieee754 type with an integer _i.
    ///@param _i The number to initialize the union via.
    explicit constexpr _ieee754_f32(const int _i) : _i(_i) {};
    ///Initialize ieee754 type with an unsigned integer _i.
    ///@param _i The number to initialize the union via.
    explicit constexpr _ieee754_f32(const unsigned int _i) : _i(_i) {};
};

#endif //F32_H

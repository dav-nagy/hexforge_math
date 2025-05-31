//
// Created by David on 5/30/2025.
//

#ifndef F32_H
#define F32_H

/*
 *Assuming little endian with 32 bit ints because frankly
 *I am too lazy not to and the only people who will be using
 *this are me and my friend anyway
 */

//IEEE754 32-bit Floating Point form
union ieee754_f32 {
    float _f;
    unsigned int _i;
    struct {
        unsigned int _mantissa : 23;
        unsigned int _exp       : 8;
        unsigned int _sgn       : 1;
    } _f_core;

    explicit constexpr ieee754_f32(const float _f) : _f(_f) {};
    explicit constexpr ieee754_f32(const int _i) : _i(_i) {};
    explicit constexpr ieee754_f32(const unsigned int _i) : _i(_i) {};
};

float inff();
float inff(bool _n); // We don't need to make this consistent with f32.cpp using const because the compiler doesn't care

float nanf(const char* _info = "");

#endif //F32_H

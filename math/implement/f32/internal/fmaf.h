//
// Created by David on 6/4/2025.
//

#ifndef FMAF_H
//Include guard
#define FMAF_H

#if defined(C_MATH_INCLUDE) && !defined(_INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fmaf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float _fmaf(float, float, float);

///Return _x * _y + _z as a ternary operation with one instance of rounding
///For implementation, see implement/f32/implement/fmaf.cpp
///@param _x One of the multiplicands, to be multiplied with _y
///@param _y Another multiplicand, to be multiplied with _x
///@param _z The independent summand, the other one being the product of _x and _y
inline float fmaf(const float _x, const float _y, const float _z)
        { return _fmaf(_x, _y, _z); }

#endif //FMAF_H

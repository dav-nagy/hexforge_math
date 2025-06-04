//
// Created by David on 6/1/2025.
//

#ifndef FABSF_H
//Include guard
#define FABSF_H

#if C_MATH_INCLUDE && !defined(_INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fabsf.h directly. Use "c_math.h" instead.
#endif

extern "C" ///Return the absolute value of a 32-bit floating point number _f
        ///That is: |_f|
        float _fabsf(float);

inline float fabsf(const float _f)
        { return _fabsf(_f); }

#endif //FABSF_H
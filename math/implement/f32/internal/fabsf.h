//
// Created by David on 6/1/2025.
//

#ifndef FABSF_H
//Include guard
#define FABSF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fabsf.h directly. Use "c_math.h" instead.
#endif

extern "C" ///Return the absolute value of a 32-bit floating point number _f
        ///That is: |_f|
        float c_fabsf(float);

#endif //FABSF_H
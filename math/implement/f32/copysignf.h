//
// Created by David on 6/25/2025.

#ifndef COPYSIGNF_H
//Include guard
#define COPYSIGNF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include copysignf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_copysignf(float, float);

#endif //COPYSIGNF_H

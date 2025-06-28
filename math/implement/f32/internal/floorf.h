//
// Created by wizard on 6/28/2025.
//

#ifndef TRUNCF_H
//Include guard
#define TRUNCF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include floorf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_floorf(float);

// inline float c_truncf(const float _f)
//         { return _c_truncf(_f); }

#endif //TRUNCF_H

//
// Created by David on 6/11/2025.
//

#ifndef FMAXF_H
//Include guard
#define FMAXF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fmaxf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_fmaxf(float, float);

#endif //FMAX_H

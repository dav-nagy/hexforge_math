//
// Created by David on 6/4/2025.
//

#ifndef FMAF_H
//Include guard
#define FMAF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fmaf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_fmaf(float, float, float);

#endif //FMAF_H

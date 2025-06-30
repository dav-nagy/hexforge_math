//
// Created by wizard on 6/28/2025.
//

#ifndef TRUNCF_H
//Include guard
#define TRUNCF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include ceilf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_ceilf(float);

#endif //TRUNCF_H

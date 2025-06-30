//
// Created by wizard on 6/29/25.
//

#ifndef ROUNDF_H
#define ROUNDF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include roundf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_roundf(float);

#endif //ROUNDF_H

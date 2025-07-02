//
// Created by David on 6/1/2025.
//

#ifndef TRUNCF_H
//Include guard
#define TRUNCF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include truncf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_truncf(float);

#endif //TRUNCF_H

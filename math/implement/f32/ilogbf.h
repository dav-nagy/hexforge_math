//
// Created by David on 6/25/2025.

#ifndef ILOGBF_H
//Include guard
#define ILOGBF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include ilogbf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        int c_ilogbf(float);

#endif //ILOGBF_H

//
// Created by David on 6/11/2025.
//

#ifndef FREXPF_H
//Include guard
#define FREXPF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include frexpf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_frexpf(float, int*);

#endif //FREXPF_H

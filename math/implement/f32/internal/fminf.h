//
// Created by David on 6/28/2025.
//

#ifndef FMINF_H
//Include guard
#define FMINF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fminf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_fminf(float, float);

#endif //FMINF_H

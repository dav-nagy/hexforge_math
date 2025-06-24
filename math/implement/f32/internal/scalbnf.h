//
// Created by David on 6/11/2025.
//

#ifndef SCALBNF_H
//Include guard
#define SCALBNF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include scalbnf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_scalbnf(float, int);

#endif //SCALBNF_H

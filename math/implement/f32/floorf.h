//
// Created by wizard on 6/28/2025.
//

#ifndef FLOORF_H
//Include guard
#define FLOORF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include floorf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_floorf(float);

#endif //FLOORF_H

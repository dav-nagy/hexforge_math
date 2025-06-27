//
// Created by wizard on 6/11/2025.
//

#ifndef MODFF_H
//Include guard
#define MODFF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include modff.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_modff(float, float*);

#endif //MODFF_H

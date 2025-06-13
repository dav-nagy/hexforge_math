//
// Created by David on 6/11/2025.
//

#ifndef NEXTAFTERF_H
//Include guard
#define NEXTAFTERF_H

#include "../../attribute/attribute.h"

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include nextafterf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_nextafterf(float, float);

#endif //NEXTAFTERF_H

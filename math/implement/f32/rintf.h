//
// Created by wizard on 6/29/25.
//

#ifndef RINTF_H
#define RINTF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include rintf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_rintf(float);

#endif //RINTF_H

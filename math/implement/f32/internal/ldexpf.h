//
// Created by David on 6/24/2025.
//

#ifndef LDEXPF_H
#define LDEXPF_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include ldexpf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float c_ldexpf(float, int);

#endif //LDEXPF_H

//
// Created by David on 6/1/2025.
//

#ifndef TRUNCF_H
//Include guard
#define TRUNCF_H

#if defined(C_MATH_INCLUDE) && !defined(_INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include truncf.h directly. Use "c_math.h" instead.
#endif

extern "C"
        float _truncf(float);

namespace hf_math {
        ///Return the smallest integral value smaller in magnitude than a 32-bit floating point number _f.
        ///@param _f The floating-point number to be truncated
        inline float truncf(const float _f)
        { return _truncf(_f); }
}

#endif //TRUNCF_H

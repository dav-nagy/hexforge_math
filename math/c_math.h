//
// Created by David on 6/3/2025.
//

#ifndef C_MATH_H
//Include Guard
#define C_MATH_H

#define C_MATH_INCLUDE


//#include "internal/hf_math.h"

namespace hf_math {
    using ::fabsf;
    using ::fmaf;
    using ::truncf;

    ///Return _x * _y + _z as a ternary operation with one instance of rounding
    ///For implementation, see implement/f32/implement/fmaf.cpp
    ///@param _x One of the multiplicands, to be multiplied with _y
    ///@param _y Another multiplicand, to be multiplied with _x
    ///@param _z The independent summand, the other one being the product of _x and _y
    inline float fma(const float _x, const float _y, const float _z)
        { return fmaf(_x, _y, _z); }


    //Testing overload (Not permanent because it is really stupid to do this
    //inline float fabsf(const double x){return fabsf(static_cast<float>(x));}
}

#undef C_MATH_INCLUDE

#endif //C_MATH_H

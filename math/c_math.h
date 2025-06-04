//
// Created by David on 6/3/2025.
//

#ifndef C_MATH_H
//Include Guard
#define C_MATH_H

#define C_MATH_INCLUDE

#include "internal/hf_math.h"

namespace hf_math {
    using ::fabsf;
    //Testing overload (Not permanent because it is really stupid to do this
    inline float fabsf(const double x){return fabsf(static_cast<float>(x));}

    using ::truncf;
}

#endif //C_MATH_H

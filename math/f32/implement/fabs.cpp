//
// Created by David on 6/2/2025.
//

#include "../fabs.h"

#include "../f32.h"

//Return the absolute value of a 32-bit floating point number _f
//That is: |_f|
float hexforge_fabs::fabs(const float _f) {
    hexforge_f32::ieee754_f32 _fx(_f);
    _fx._i &= 0x7fffffff; // 32-bit floating point exp/mantissa mask
    return _fx._f;
}

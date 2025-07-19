//
// Created by David on 5/31/2025.
//

#ifndef F32_FUNCTIONS_H
//Include guard
#define F32_FUNCTIONS_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include f32_functions.h directly. Use "c_math.h" instead.
#endif

extern "C" {
    float c_inff();
    float c_ninff(bool);

    bool c_is_pinff(float);
    bool c_is_ninff(float);
    bool c_is_inff(float);

    float c_nanf(const char*, bool);

    bool c_is_qnanf(float);
    bool c_is_snanf(float);
    bool c_is_nanf(float);

    bool c_is_finitef(float);
    bool c_is_normalf(float);
    int c_signbitf(float);

    int c_fpclassifyf(float);
}

#endif //F32_FUNCTIONS_H

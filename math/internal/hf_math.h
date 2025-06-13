//
// Created by David on 6/3/2025.
//

#ifndef HF_MATH_H
//Include guard
#define HF_MATH_H

extern "C"{
    float c_fabsf(float);

    float c_fmaf(float, float, float);

    float c_frexpf(float, int*);

    float c_inff();
    float c_ninff(bool);

    bool c_is_inff(float);
    bool c_is_ninff(float);
    bool c_is_pinff(float);

    bool c_is_nanf(float);
    bool c_is_qnanf(float);
    bool c_is_snanf(float);

    float c_nextafterf(float, float);

    float c_nanf(const char*, bool);

    float c_truncf(float);
}

#endif //HF_MATH_H

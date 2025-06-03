//
// Created by David on 5/31/2025.
//

#ifndef F32_FUNCTIONS_H
//Include guard
#define F32_FUNCTIONS_H

#include "f32.h"
#include "../char/nan_helper.h"

namespace hexforge_f32_functions {
    float inff();
    float inff(bool);

    bool is_pinff(float);
    bool is_ninff(float);
    bool is_inff(float);

    float nanf(const char*, bool);

    bool is_qnanf(float);
    bool is_snanf(float);
    bool is_nanf(float);
}

#endif //F32_FUNCTIONS_H

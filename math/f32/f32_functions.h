//
// Created by David on 5/31/2025.
//

#ifndef F32_FUNCTIONS_H
#define F32_FUNCTIONS_H

#include "f32.h"
#include "../char/nan_helper.h"

float inff();
float inff(bool _n); // We don't need to make this consistent with f32.cpp using const because the compiler doesn't care

float nanf(const char* _msg, bool _quiet);

bool is_qnanf(float _f);
bool is_snanf(float _f);

#endif //F32_FUNCTIONS_H

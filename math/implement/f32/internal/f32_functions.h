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
}

// inline float c_inff()
//     { return _c_inff(); }
// inline float c_ninff(const bool _n)
//     { return _c_ninff(_n); }
//
// inline bool c_is_pinff(const float _f)
//     { return _c_is_pinff(_f); }
// inline bool c_is_ninff(const float _f)
//     { return _c_is_ninff(_f); }
// inline bool c_is_inff(const float _f)
//     { return _c_is_inff(_f); }
//
// inline float c_nanf(const char* _s, const bool _q)
//     { return _c_nanf(_s, _q); }
//
// inline bool c_is_qnanf(const float _f)
//     { return _c_is_qnanf(_f); }
// inline bool c_is_snanf(const float _f)
//     { return _c_is_snanf(_f); }
// inline bool c_is_nanf(const float _f)
//     { return _c_is_nanf(_f); }


#endif //F32_FUNCTIONS_H

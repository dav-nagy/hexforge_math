//
// Created by David on 6/2/2025.
//

#ifndef FABS_H
//Include guard
#define FABS_H

extern "C"{
    float _ieee_fabsf(float);
    float _fabsf(float);
}

namespace hf_math {
    inline float fabsf(const float _f) {
        return ::_fabsf(_f);
    }
}

#endif //FABS_H

//
// Created by David on 6/1/2025.
//

#ifndef FABSF_H
//Include guard
#define FABSF_H

extern "C"{
    float _ieee754_fabsf(float);

    float _fabsf(float);
}

namespace hf_math {
    inline float fabsf(const float _f) {
        return ::_fabsf(_f);
    }
}

#endif //FABSF_H
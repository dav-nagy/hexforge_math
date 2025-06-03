//
// Created by David on 6/1/2025.
//

#ifndef TRUNC_H
//Include guard
#define TRUNC_H

extern "C"{
    float _ieee754_trunc(float);
    float _truncf(float);
}

namespace hf_math {
    inline float truncf(const float _f) {
        return ::_truncf(_f);
    }
}

#endif //TRUNC_H

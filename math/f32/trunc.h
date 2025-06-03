//
// Created by David on 6/1/2025.
//

#ifndef TRUNC_H
//Include guard
#define TRUNC_H

extern "C"{
    float _ieee754_trunc(float);

    float truncf(float);
}

namespace _hf {
    inline float truncf(const float _f) {
        return ::truncf(_f);
    }
}

#endif //TRUNC_H

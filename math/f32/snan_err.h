//
// Created by David on 6/1/2025.
//

#ifndef SNAN_ERR_H
#define SNAN_ERR_H
#include "f32_functions.h"

namespace hexforge_snan_err {
    inline void throw_snan(const float _f) {
        if (hexforge_f32_functions::is_snanf) throw "Signaling NaN detected";
    }
}

#endif //SNAN_ERR_H

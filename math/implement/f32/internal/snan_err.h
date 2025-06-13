//
// Created by David on 6/1/2025.
//

#ifndef SNAN_ERR_H
//Include guard
#define SNAN_ERR_H
#include "f32_functions.h"

namespace hexforge_snan_err {
    //Detect if a given 32-bit float _f is a Signaling NaN
    //If so, throw an exception
    //This function is designed to be implemented in a try{...}catch(...){...} environment
    inline void throw_snan(const float _f) {
        if (c_is_snanf(_f)) throw "Signaling NaN detected";
    }
}

#endif //SNAN_ERR_H

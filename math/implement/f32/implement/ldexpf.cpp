//
// Created by David on 6/24/2025.
//

/*
 * This is just a wrapper for scalbnf, as on a radix-2 system they are equivalent.
 */

#define INTERNAL_CPP
#include "../internal/ldexpf.h"
#include "../internal/scalbnf.h"
#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

extern "C"
    _internal
    float _ieee754_ldexpf(const float _x, const int _exp) {
    return c_scalbnf(_x, _exp);
}

extern "C"
    _strong_alias(c_ldexpf, _ieee754_ldexpf);
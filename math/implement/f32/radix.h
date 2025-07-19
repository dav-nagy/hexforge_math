//
// Created by David on 6/24/2025.
//

#ifndef RADIX_H
#define RADIX_H
#include "../../../io/io.h"

/*
 * The following is to verify that the system radix is 2.
 * We check the change in exponent of a floating-point number to determine this.
 */

static struct _radix_error {
    const char *_msg;
    explicit _radix_error(const char* _x) : _msg(_x) {}
} _err("");

static unsigned int float_to_int_internal(float _x) {
    const union {float _f; unsigned int _i; } _fx = {_x};
    return _fx._i;
}

static int is_radix_two_internal() {
    const unsigned int _one = float_to_int_internal(1.0f),
                       _two = float_to_int_internal(2.0f);
    if(_one == (0x7f << 23) && _two == (0x80 << 23))
        return 1;
    return 0;
}

inline static struct check_radix {
    check_radix() {
        if (!is_radix_two_internal()) {
            cio_out << "[FATAL] Unsupported floating-point radix; expected 2 (IEEE-754).\n";
            throw _err; //Intentionally uncaught to terminate the program
        }
    }
} _check_radix;
#endif //RADIX_H

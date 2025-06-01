#include <iostream>

#include "math/f32/f32.h"
#include "math/f32/f32_functions.h"

int main() {
    const char* _c = "-1";
    const ieee754_f32 _s(nanf(_c, false));
    std::cout << parse_nan_char(_c) << std::endl;
    std::cout << is_qnanf(_s._f) << '\n';
    std::cout << _s._f << '\n';
    std::cout << _s._i << '\n';

    try {
        throw "Signaling NaN detected!";
    } catch(const char* _e) {
        std::cout << _e << ", Scary!" << '\n';
    }
    return 0;
}

#include <iostream>

#include "math/f32/f32.h"

int main() {
    constexpr ieee754_f32 _f(1.5f);
    std::cout << _f._i << std::endl;
    const char* msg = "Hei...";
    const ieee754_f32 nan(nanf(msg));
    std::cout << ieee754_f32(nanf(msg))._i << std::endl;
    const char* message = reinterpret_cast<const char*>(nan._i & 0x3fffff << 10);
    std::cout << message << std::endl;
    return 0;
}
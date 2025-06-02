#include <iostream>
#include <iomanip>

#include "io/io.h"
#include "math/f32/f32_functions.h"
#include "math/f32/fabs.h"
#include "math/f32/trunc.h"
#include "math/int/trailing_zeros.h"
#include "string/to_string.h"

int main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';

    //hexforge_io::cio_out << static_cast<int>(hexforge_trunc::truncf(511.70f));
    io::cio_out << "truncf(\"" << "\") = " << static_cast<int>(hexforge_trunc::truncf(512938.344f)) << '\n';
    std::cout << std::setprecision(16) << hexforge_fabs::fabs(-hexforge_f32_functions::inff(true)) << '\n';

    io::cio_out << hexforge_trailing_zeros::trailing_zeros(16) << '\n';
    io::cio_out << static_cast<signed int>(hexforge_f32::ieee754_f32(hexforge_f32_functions::nanf("729", true))._i);

    //↓↓↓ Spooky scary... do not interact with for the time bing ↓↓↓
    __asm__("");
    /////////////////////
    return 0;
}

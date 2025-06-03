#include <iostream>
#include <iomanip>
//
// #include "io/io.h"
// #include "math/f32/f32_functions.h"
// #include "math/f32/fabs.h"
// #include "math/f32/trunc.h"
// #include "math/int/trailing_zeros.h"
// #include "string/to_string.h"

#include <cmath>

#include "math/f32/f32.h"
#include "math/f32/trunc.h"

int main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';

    //hexforge_io::cio_out << static_cast<int>(hexforge_trunc::truncf(511.70f));
   // io::cio_out << "truncf(\""/*Not putting the float here because dragonbox is hard :(*/ << "\") = " <<
   //     static_cast<int>(_hf::truncf(8388609.3f)) << '\n';
   // std::cout << std::setprecision(16) << hexforge_fabs::fabs(-hexforge_f32_functions::inff(true)) << '\n';


        std::cout << std::setprecision(30) << hexforge_f32::ieee754_f32(_hf::truncf(2.23423447541488131233e-39))._i << '\n';

    //io::cio_out << hexforge_trailing_zeros::trailing_zeros(16) << '\n';
    //io::cio_out << static_cast<signed int>(hexforge_f32::ieee754_f32(hexforge_f32_functions::nanf("729", true))._i) << '\n';

   // char buffer[128];
    //hexforge_cio_in::cio_read(buffer, 128);
    //io::cio_out << buffer << '\n';
    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    __asm__("");
    /////////////////////
    return 0;
}

#include <iostream>
#include <iomanip>
//
// #include "io/io.h"
// #include "math/f32/f32_functions.h"
// #include "math/f32/fabs.h"
// #include "math/f32/trunc.h"
// #include "math/int/trailing_zeros.h"
// #include "string/to_string.h"

#include "io/io.h"
#include "io/input_output/ops/cio_out_ops.h"
#include "math/f32/f32.h"
#include "math/f32/fabs.h"
#include "math/f32/trunc.h"


int main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';


    std::cout << hf_math::truncf(-93847.324337f);

    char buffer[128];
    hexforge_cio_in::cio_read(buffer, 128);
    io::cio_out << buffer << '\n';
    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    __asm__("");
    ////////////////////
    return 0;
}

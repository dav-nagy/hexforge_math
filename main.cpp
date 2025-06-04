#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

//#include "math/implement/f32/internal/fabsf.h"

#include "math/c_math.h"


int main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';

    std::cout << std::setprecision(16) << fabsf(-93847.324337f) << '\n';
    std::cout << std::setprecision(16) << truncf(-93847.324337f) << '\n';



    //char buffer[128];
    //hexforge_cio_in::cio_read(buffer, 128);
    //io::cio_out << buffer << '\n';
    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    __asm__("");
    ////////////////////
    return 0;
}

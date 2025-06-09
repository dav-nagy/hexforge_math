
#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

#include <random>

#include "math/c_math.h"

int main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';

    //std::cout << std::setprecision(16) << fabsf(-93847.324337f) << '\n';
    //std::cout << std::setprecision(16) << truncf(-93847.324337f) << '\n';

    float
        x = 1.0e-30f,
        y = 1.0e-10f,
        z = 1.0e-40f;

    float _max_err = 0.0f;
    float _w_x = 0, _w_y = 0, _w_z = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 0xffffffff);

    // for (int i = 0; i <= 1e7; ++i) {
    //     float _x = std::bit_cast<float>(dist(gen));
    //           //_y = std::bit_cast<float>(dist(gen));
    //           //_z = std::bit_cast<float>(dist(gen));
    //     float _fi = hf_math::truncf(_x), //My implementation
    //           _fc = std::truncf(_x); //<cmath>
    //     float _err = _fi - _fc;
    //     if (std::isnan(_err)) _err = (0);
    //     if (_err > _max_err) {
    //         _max_err = _err;
    //         _w_x = _x;
    //         //_w_y = _y;
    //         //_w_z = _z;
    //     }
    // }
    // float _wfi = truncf(_w_x), //My implementation
    //           _wfc = std::truncf(_w_x); //<cmath>
    // std::cout << "Max error: " << _max_err << '\n';
    // std::cout << "Worst Offender: \n";
    // std::cout << "truncf(" << _w_x << ")" << '\n';
    // std::cout<< "===========================================================\n";
    // std::cout << "Impl:    " << std::setprecision(30) << _wfi << '\n';
    // std::cout << "<cmath>: " << std::setprecision(30) << _wfc << '\n';
    // std::cout << "==========================================================\n";
    // (double)_e   << '\n' << _rd; //Double precision result

    std::cout << std::setprecision(30) << fmaf(x, y, z) << std::endl;

    //char buffer[128];
    //hexforge_cio_in::cio_read(buffer, 128);
    //io::cio_out << buffer << '\n';
    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    asm("");
    ////////////////////

    return 0;
}

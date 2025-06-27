#include <cmath>
#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

#include <chrono>
#include <random>

#include "math/c_math.h"
#include "math/implement/f32/internal/f32.h"
#include "math/implement/f32/internal/round_mode.h"
#define MATH_INCLUDE
#undef MATH_INCLUDE

inline void print_round() {
    switch (c_get_f_round()) {
        case _round_up:
            std::cout << "Upward\n";
            break;
        case _round_down:
            std::cout << "Downward\n";
            break;
        case _round_nearest:
            std::cout << "Nearest\n";
            break;
        case _round_zero:
            std::cout << "Zero\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
    }
}

// #define _func(t, ...) nextafter(##__VA_ARGS__ )
// template<typename... _args, typename _arg_ty>
//     inline _arg_ty _i_func(_args... _arg)
// { return _func(_arg_ty, _arg); }
// #define func(namespace, ...) namespace::_func(_, ##__VA_ARGS__)

int not_main();

#define _int64 long long
//
// namespace evil_memcpy {
//     extern "C" void* evil_memcpy(void *, const void *, unsigned _int64); //Will implement in asm later...
// }

int main() {
    (void) not_main();
    std::cout << (1) << '\n';
    // std::cout << add64(1, 2) << '\n';
    // std::cout << f32_to_int32(2.75) << '\n';
    // std::cout << int32_to_f32(1076887552) << '\n';

    return 0;
}

int not_main() {

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

    long int _max_err = 0.0f;
    float _w_x = 0, _w_y = 0, _w_z = 0;
    int _w_e;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dist;

    srand(time(nullptr));

    for (int i = 0; i <= 1e7; ++i) {
        unsigned int _i = dist(gen);
        unsigned int _j = dist(gen);
        unsigned int _k = dist(gen);
        // _i %= 8388608; //ONly subnormals
        float _x = * reinterpret_cast<float *> (&_i),
              _y = * reinterpret_cast<float *> (&_j),
              _z = * reinterpret_cast<float *> (&_k);
        int _e = ((rand() % 127 + 1) << 1) - 127; //Narrowing conversion does not matter because this value is supposed to be random anyway...
        float _fi = hf_math::ilogb(_x), //My implementation
              _fc = std::ilogb(_x); //<cmath>
        _ieee754_f32 _ifi(_fi), _ifc(_fc);
        long int _err = _ifi._i - _ifc._i;
        if ((std::isinf(_fi) && std::isinf(_fc)) || (std::isnan(_fi) && std::isnan(_fc))) _err = 0;
        if (std::abs(_err) > std::abs(_max_err)) {
            _max_err = _err;
            _w_x = _x;
            _w_y = _y;
            _w_z = _z;
            _w_e = _e;
        }
    }
                  //   _w_z = -1.330263982701109876446170975626742031745016031545105621906736154149678182101457224462137673981487751007080078125e-38f;
    float _wfi = hf_math::ilogb(_w_x), //My implementation
          _wfc = std::ilogb(_w_x); //<cmath>
    std::cout << "Max error: " << _max_err << "ulp\n";
    std::cout << "Worst Offender: \n";
    std::cout << std::setprecision(300)<<"ilogbf(" << _w_x << ")" << '\n';
    std::cout << "===========================================================\n";
    _ieee754_f32 _iwfi(_wfi), _iwfc(_wfc);
    std::cout << "Impl:    " << std::setprecision(300) << _iwfi._f << '\n';
    std::cout << "<cmath>: " << std::setprecision(300) << _iwfc._f << '\n';
    std::cout << "===========================================================\n";
    //std::cout << hf_math::inf(false);
    //std::cout << 249343001288519586036853764895746818048.f << '\n';
    //(double)_e   << '\n' << _rd; //Double precision result

  //  std::cout << std::setprecision(30) << fmaf(x, y, z) << std::endl;
  //  std::cout << std::setprecision(30) << hf_math::trunc(239.27912);

    //char buffer[128];
    //hexforge_cio_in::cio_read(buffer, 128);
    //io::cio_out << buffer << '\n';
    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    asm("");
    ////////////////////

    return 0;
}

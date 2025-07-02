#include <cmath>
#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

#include <cfenv>
#include <chrono>
#include <random>

#include "math/c_math.h"
#include "math/implement/f32/internal/f32.h"
#include "math/implement/round_mode.h"
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
int not_main();

#define _int64 long long

#include "math/implement/f32/internal/sqrtf.h"

int main() {
    // float x;
    // std::cout << "Enter the number to get the square root of: \n";
    // std::cin >> x;
    // std::cout << std::setprecision(300) << c_sqrtf(x) << '\n';
    // std::cout << std::setprecision(300) << std::sqrt(x) << '\n';
    fesetround(FE_TONEAREST);
    (void) not_main();
    fesetround(FE_UPWARD);
    (void) not_main();
    fesetround(FE_DOWNWARD);
    (void) not_main();
    fesetround(FE_TOWARDZERO);
    (void) not_main();
    //std::cout << hf_math::rint(3.5f) << std::endl;
    return 0;
}

int not_main() {

    // cio_write("Test!!\n");
    // cio_out _r;
    // _r << "new" << '\n' << "line" << "\n";
    // const string _s = to_string::to_string(320);
    // _r << _s.data() << '\n';

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

    for (unsigned int i = 0; i <= 1e7; ++i) {
        unsigned int _i = dist(gen);
        unsigned int _j = dist(gen);
        unsigned int _k = dist(gen);
        // _i %= 8388608; //Only subnormals
        _i = i << 11;
        float _x = * reinterpret_cast<float *> (&(_i)),
              _y = * reinterpret_cast<float *> (&_j),
              _z = * reinterpret_cast<float *> (&_k);
        int _e = ((rand() % 127 + 1) << 1) - 127; //Narrowing conversion does not matter because this value is supposed to be random anyway...
        float _fi = hf_math::sqrt(_x), //My implementation
              _fc = std::sqrt(_x); //<cmath>
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
    float _wfi = hf_math::sqrt(_w_x), //My implementation
          _wfc = std::sqrt(_w_x); //<cmath>
    std::cout << "Max error: " << _max_err << "ulp\n";
    std::cout << "Worst Offender: \n";
    std::cout << std::setprecision(300)<<"rint(" << _w_x << ")" << '\n';
    std::cout << "===========================================================\n";
    _ieee754_f32 _iwfi(_wfi), _iwfc(_wfc);
    std::cout << "Impl:    " << std::setprecision(300) << _iwfi._f << '\n';
    std::cout << "<cmath>: " << std::setprecision(300) << _iwfc._f << '\n';
    std::cout << "===========================================================\n";

    //↓↓↓ Spooky scary... do not interact with lest it become enraged ↓↓↓
    asm("");
    ////////////////////

    return 0;
}

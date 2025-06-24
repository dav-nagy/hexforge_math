#include <cmath>
#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

#include <random>

#include "math/c_math.h"
#include "math/implement/f32/internal/f32.h"
#include "math/implement/f32/internal/numbers.h"
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

float musl_scalbnf(float x, int n)
{
    union {float f; uint32_t i;} u;
    float_t y = x;

    if (n > 127) {
        y *= 0x1p127f;
        n -= 127;
        if (n > 127) {
            y *= 0x1p127f;
            n -= 127;
            if (n > 127)
                n = 127;
        }
    } else if (n < -126) {
        y *= 0x1p-126f * 0x1p24f;
        n += 126 - 24;
        if (n < -126) {
            y *= 0x1p-126f * 0x1p24f;
            n += 126 - 24;
            if (n < -126)
                n = -126;
        }
    }
    u.i = (uint32_t)(0x7f+n)<<23;
    x = y * u.f;
    return x;
}

// #define _func(t, ...) nextafter(##__VA_ARGS__ )
// template<typename... _args, typename _arg_ty>
//     inline _arg_ty _i_func(_args... _arg)
// { return _func(_arg_ty, _arg); }
// #define func(namespace, ...) namespace::_func(_, ##__VA_ARGS__)

#include <cfenv>

int not_main();

extern "C"
float _dekker_m(const float x, const float y, volatile float* _err) {
constexpr float C = ((1 << (_flt_mant_len + 1) / 2) + 1);
    float x1 = x * C;
    float y1 = y * C;
    const float m1 = x * y;
    x1 = (x - x1) + x1;
    y1 = (y - y1) + y1;
    const float x2 = x - x1;
    const float y2 = y - y1;
    *_err = (((x1 * y1 - m1) + x1 * y2) + x2 * y1) + x2 * y2;
    return m1;
}

// union _union_int24 {
//     unsigned long _i : 24;
// };

#define _type(x) __typeof__(x)
#define _expand_t(x) x
#define _long_str(x) #x


int main() {
    (void) not_main();
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

    for (int i = 0; i <= 100; ++i) {
        unsigned int _i = dist(gen);
        unsigned int _j = dist(gen);
        unsigned int _k = dist(gen);
        // _i %= 8388608; //ONly subnormals
        float _x = * reinterpret_cast<float *> (&_i),
              _y = * reinterpret_cast<float *> (&_j),
              _z = * reinterpret_cast<float *> (&_k);
        int _e = ((rand() % 127 + 1) << 1) - 127; //Narrowing conversion does not matter because this value is supposed to be random anyway...
        float _fi = hf_math::ldexp(_x, _e), //My implementation
              _fc = std::ldexp(_x, _e); //<cmath>
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
    float _wfi = hf_math::ldexp(_w_x, _w_e), //My implementation
          _wfc = std::ldexp(_w_x, _w_e); //<cmath>
    std::cout << "Max error: " << _max_err << "ulp\n";
    std::cout << "Worst Offender: \n";
    std::cout << std::setprecision(300)<<"ldexpf(" << _w_x << ", " << _w_e << ")" << '\n';
    std::cout << "===========================================================\n";
    _ieee754_f32 _iwfi(_wfi), _iwfc(_wfc);
    std::cout << "Impl:    " << std::setprecision(300) << _iwfi._f << '\n';
    std::cout << "<cmath>: " << std::setprecision(300) << _iwfc._f << '\n';
    std::cout << "musl:    " << std::setprecision(300) << musl_scalbnf(_w_x, _w_e);
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

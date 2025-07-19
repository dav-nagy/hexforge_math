#include <cmath>
#include <iostream>
#include <iomanip>

#define SKIP_WARN
#undef SKIP_WARN

#include <cfenv>
#include <chrono>
#include <random>

#define MATH_INCLUDE


#include "math/c_math.h"
#include "math/implement/f32/internal/f32.h"
#include "math/implement/f32/internal/numbers.h"
#include "math/implement/round_mode.h"
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

extern "C" int asm_ilogbf(float);

#define _int64 long long
float __attribute__((always_inline)) asm_fabs(float x) {
   float _r;
   __asm__ __volatile__ (
      "movd %1, %%ecx\n\t"
      "and $0x7fffffff, %%ecx\n\t"
      "movd %%ecx, %0"
      : "=x" (_r)
      : "x" (x)
      : "ecx"
   );
   return _r;
}
float __attribute__((always_inline)) asm_copysign(float x, float y) {
   float _r;
   __asm__ __volatile__ (
      "movd %1, %%ecx\n\t"
      "movd %2, %%edx\n\t"
      "and $0x7fffffff, %%ecx\n\t"
      "and $0x80000000, %%edx\n\t"
      "or %%edx, %%ecx\n\t"
      "movd %%ecx, %0"
      : "=x" (_r)
      : "x" (x), "x" (y)
      : "ecx", "edx"
   );
   return _r;
}

#include "math/implement/attribute/attribute.h"
void __attribute__((always_inline))
   _deprecated("24/1/2010", "asm_print is not actually deprecated, and this is just a test...")
   asm_print(const char* msg) {
   unsigned _int64 _sz = 0;
   while (msg[_sz]) _sz++;

   __asm__ __volatile__ (
      "mov $1, %%rax\n\t" //syscall
      "mov $1, %%rdi\n\t" //fd = 1
      "movq %0, %%rsi\n\t" //buf ptr
      "movq %1, %%rdx\n\t" //buf size
      "syscall \n\t"
      :
      : "r" (msg), "r" (_sz)
      : "rax", "rdi", "rsi", "rdx"
   );
}

struct double_float {
   float _hi = 0.0f, _lo = 0.0f;
   explicit double_float(const float _x)
      : _hi(_x) {}
};

inline double_float split(const float x) {
   float _x = x;
   float _c = _x * 4097.f;
   double_float _r(_c - (_c - _x));
   _r._lo = _x - _r._hi;
   return _r;
}

double_float mul12(const float x, const float y) {
   double_float _x = split(x),
                _y = split(y);
   double_float _r(x * y);
   _r._lo = ((_x._hi * _y._hi - _r._hi) + _x._hi * _y._lo + _x._lo * _y._hi) + _x._lo * _y._lo;
   return _r;
}

inline double_float dekker_square(const float _x) {
   float _xh = (_x - _x * 4097.0f) + _x * 4097.0f;
   const float _xl = _x - _xh;
   const float _x2h = _x * _x,
   _x2l =
      ((_xh * _xh - _x2h) + 2.0f * _xh * _xl) + _xl * _xl;
   double_float _r(_x2h);
   _r._lo = _x2l;
   return _r;
}

#include "core-math/cbrtf.h"

int main() {
   // constexpr float x = 3904341631041536.f;
   // constexpr float y = 174305338393911634165760.f;
   // constexpr float z = -2843053391872.f;
   asm_print("syscall\n");
   asm_print("asm_print()\n__asm__ __volatile__ (\n   *\n)\n");
   asm_print("Jesse, we need to cook!\n\n\n");

   //std::cout << std::setprecision(300) << c_cbrtf(flt_sub_epsilon) << '\n';
   //std::cout << std::setprecision(300) << c_cbrtf(125.0f);

   fesetround(FE_UPWARD);

   float evil = math_pi_f;
      double_float _q = dekker_square(evil);
   std::cout << std::setprecision(300) << "evil^2 with dekker = " << _q._hi << " + " << _q._lo << '\n';
   double_float _r(evil * evil);
   _r._lo = hf_math::fma(evil, evil, -_r._hi);
   std::cout << std::setprecision(300) << "evil^2 with fma    = " << _r._hi << " + " << _r._lo << '\n';

   for (float _f = 1.0f; _f < 8.0f; _f = std::nextafterf(_f, __builtin_inff())) {
      if (std::isnan(cr_cbrtf(_f)) && std::isnan(hf_math::cbrt(_f)))
         continue;
      if (cr_cbrtf(_f) != hf_math::cbrt(_f)) {
         float x = hf_math::cbrt(_f);
         std::cout << "Incorrect!\n";
      }
   }

   //std::cout << std::setprecision(300) << c_cbrtf(-flt_max);

   // fesetround(FE_TOWARDZERO);
   // std::cout << "David's fma implementation:     " << std::setprecision(300) << hf_math::fma(x, y, z) << '\n';
   // std::cout << "<cmath>'s fma implementation:   " << std::setprecision(300) << std::fma(x, y, z) << '\n';
   // std::cout << "Naive x * y + z implementation: " << std::setprecision(300) << x * y + z << '\n';
   // fesetround(FE_TONEAREST);
   // (void)not_main();
   // fesetround(FE_UPWARD);
   // (void)not_main();
   // fesetround(FE_DOWNWARD);
   // (void)not_main();
   // fesetround(FE_TOWARDZERO);
   // (void)not_main();
   // (void)std::sin(-3.0);
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

    long long _max_err = 0;
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
        float _x = * reinterpret_cast<float *> (&(_i)),
              _y = * reinterpret_cast<float *> (&_j),
              _z = * reinterpret_cast<float *> (&_k);
        int _e = ((rand() % 127 + 1) << 1) - 127; //Narrowing conversion does not matter because this value is supposed to be random anyway...
        float _fi = hf_math::fma(_x, _y, _z), //My implementation
              _fc = std::fma(_x, _y, _z); //<cmath>
        _ieee754_f32 _ifi(_fi), _ifc(_fc);
        long long _err = _ifi._i - _ifc._i;
        if ((std::isinf(_fi) && std::isinf(_fc)) || (std::isnan(_fi) && std::isnan(_fc))) _err = 0;
        if (std::abs(_err) > std::abs(_max_err)) {
            _max_err = _err;
            _w_x = _x;
            _w_y = _y;
            _w_z = _z;
            _w_e = _e;
        }
    }

   float _wfi = hf_math::fma(_w_x, _w_y, _w_z), //My implementation
          _wfc = std::fma(_w_x, _w_y, _w_z); //<cmath>
    std::cout << "Max error: " << _max_err << "ulp\n";
    std::cout << "Worst Offender: \n";
    std::cout << std::setprecision(300)<<"fmaf(" << _w_x << ", " << _w_y << ", " << _w_z << ")" << '\n';
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

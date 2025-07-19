   //
   // Created by wizard on 6/29/25.
   //

   /*
    * 32-bit rintf() function _ieee754_rintf()
    *      Returns _f rounded to the nearest integer, according
    *      to the current rounding mode.
    *
    *  Method:
    *      This is a very similar method to that used in glibc and musl.
    *
    *      If _f is already large (>= 0x1p23f), it will always
    *      be integral/inf/NaN, and we can return early
    *
    *      Otherwise, we multiply by 0x1p23f to intentionally lose precision.
    *      This makes the intermediate value always integral. Then, we
    *      subtract 0x1p23f to bring it back down into range, but this time
    *      it is an ineger determined by the current rounding mode.
    *
    *      Now, a compiler will sometimes optimize away an operation
    *      immediately followed by its inverse, because mathemtaically
    *      they cancel out. However, we don't want this, so setting
    *      the value of a variable as 0x1p23f removes this optimization.
    */

   #define INTERNAL_CPP
   #include "../rintf.h"
#include "../f32.h"
#undef INTERNAL_CPP

   #include "../../attribute/attribute.h"

   static constexpr float _exp2_23[2] =
       {0x1p23f, -0x1p23f};

   extern "C"
       _internal
       float _ieee754_rintf(const float _f){
       _ieee754_f32 _fx(_f);
       if (_fx._f_core._exp >= 150) //_f is always integral/inf/NaN
           return _fx._f;
       const unsigned int _s = _fx._f_core._sgn;
       _fx._f = _fx._f + _exp2_23[_s] - _exp2_23[_s];
       if (_fx._f == 0.0f) { //Restore signed zero if it was lost
           return _s ? -0.0f : 0.0f;
       }
       return _fx._f;
   }

   extern "C"
       _strong_alias(c_rintf, _ieee754_rintf);

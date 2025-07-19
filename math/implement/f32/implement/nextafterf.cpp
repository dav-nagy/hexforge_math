//
// Created by David on 6/11/2025.
//

#define INTERNAL_CPP
#include "../nextafterf.h"
#include "../../attribute/attribute.h"
#include "../f32.h"
#include "../numbers.h"
#undef INTERNAL_CPP

extern "C"
    _internal
    float _ieee754_nextafterf(const float _x, const float _y) {
    //To access the floating-point data
    _ieee754_f32 _ix(_x), _iy(_y);
    const unsigned long _aix = _ix._i & flt_abs_mask, //|_x|'s bitfield
                        _aiy = _iy._i & flt_abs_mask; //|_y|'s bitfield
    //If _x or _y is NaN
    if(_aix > flt_inf_bits || _aiy > flt_inf_bits)
        return _x + _y;
    //This is an easy case.
    //If _x == _y, return either one (in this case, _y)
    if (_ix._i == _iy._i)
        return _iy._f;
    //If _x = 0 then the next float after will be +-_flt_sub_epsilon
    //Depending on the sign of _y
    if (_aix == 0) {
        _ix._i = (_iy._i & flt_sgn_mask) | 1; //flt_sub_epsilon OR sgn bit
        return _ix._f;
    }
    //If _x is positive
    if (_ix._i < flt_sgn_mask) {
        //If _y is also positive, and greater than _x, we increment _x
        if (_iy._i < flt_sgn_mask && _ix._i < _iy._i)
            //In a case where the mantissa is all ones,
            //this is actually good because it will increase the exponent and roll over the mantissa
            _ix._i ++;
        else //When _y is negative, or it is less than _x, we decrement _x
            _ix._i --;
    }
    else { //This is when _x is negative
        //If _y is also negative, and it is greater than _x in magnitude
        //We increment _x because _y is further from zero, and decrementing a negative will move _x from zero
        if (_iy._i < flt_sgn_mask && _ix._i < _iy._i)
            _ix._i ++;
        else { //_y is positive, or it is smaller than _x in magnitude, we increment _x
            _ix._i --;
        }
    }
   //Final result ~ no more tweaking necessary!
    return _ix._f;
}

extern "C"
    _strong_alias(c_nextafterf, _ieee754_nextafterf);


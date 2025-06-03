    //
    // Created by David on 6/1/2025.
    //

    #include "../trunc.h"
    #include "../f32.h"

    /**
     * Basically, an integer will never have bits past the
     * nth leading mantissa bit, where n is the exponent.
     * This lets us create a mask by getting all 1's int the
     * mantissa, then shifting it by 23 - n, because the
     * mantissa is 22 bits long.
     *
     *=====================================================
     *
     * Demonstration:
     * 511.70f :
     * 0 - 10000111 - 11111111101100110011010
     * ^      ^                  ^
     *sign   exp             mantissa
     * AND the mantissa with 11111111100000000000000
     * 0 - 10000111 - 11111111101100110011010
     * &              11111111100000000000000
     * We return 511.0f, which is this in binary:
     * 0 - 10000111 - 11111111100000000000000
     * The double implementation is similar to this, but the mantissa
     * is 52 bits wide, and there is a different exponent bias.
     */


#ifndef ATTRIBUTE_H
#include "../../attribute/attribute.h"
#endif

#ifndef _internal_hidden
    #include "../../attribute/attribute.h"
#endif
extern "C" _internal_hidden // Some evil gatekeeping to keep the public API clean
    float
        _ieee754_truncf(const float _f) {//This is not in any namespace as to avoid name mangling (Thanks, C++)

    //Return the smallest integral value smaller than a 32-bit floating point number _f
    //By smaller, I mean it rounds towards zero (e.g. truncf(-3.75f) = -3, not -4)
    hexforge_f32::ieee754_f32 _fx(_f);
    const unsigned int _sgn = _fx._i & 0x80000000; //Isolate the sign bit, should have 31 trailing zeros
    //Detect subnormal input
    if (_fx._f_core._exp == 0) {
        _fx._i &= 0x80000000; //We can do this because subnormal floats trunc to zero,
                             //and this is the sign bit mask which results in zero and an isolated sign bit.
        return _fx._f;
    }
    // |_f|
    _fx._i &= 0x7fffffff; // Mask out the sign bit, easy
    //If |_f| is greater than this, it's an integer, infinity, or NaN
    if(_fx._i > 0x4affffff) //Bit-representation of floating-point 2^23-ulp (i.e. the largest decimal representable in 32 bits)
                            //That's right. 0x4b7fffff, which was used last year, was not even the correct number!
        return _f;
    const unsigned int _e = _fx._f_core._exp - 0x07f; // Subtract Exponent bias
    _fx._f_core._mantissa &= ((1 << (_e + 1)) - 1) << (0x017 /*23*/ - _e); // Mantissa bit mask as implemented above...
    _fx._i |= _sgn; //Restore the sign bit of _fx
    return _fx._f;
}

    extern "C" { //This must go in extern "C" to avoid name mangling
        //Format it nicely for the public API
        _strong_alias(_truncf, _ieee754_truncf);
    }
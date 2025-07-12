//
// Created by David on 6/4/2025.
//

#define INTERNAL_CPP
#include "../internal/fmaf.h"
#include "../../attribute/attribute.h"
#include "../../round_mode.h"
#include "../internal/f32.h"
#include "../internal/force_eval.h"
#include "../internal/numbers.h"
#undef INTERNAL_CPP

/*
 * This fmaf is a modified version of Norbert Juffa's fmaf implementation in response
 * to a stackoverflow question:
 *    https://stackoverflow.com/a/73841277
 *
 *    It uses integer math on mantissas and exponents, just like how a cpu
 *    does floating-point ops under the hood. It stores _x * _y in 64 bits
 *    so we have precision to add _z. Rounding and sticky bits are tracked
 *    in this code nicely. To port it to double, you use to long long's to
 *    represent a 128-bit integer.
 */

static constexpr unsigned int one_mant = 0x800000; // (1 << 23)
#define _int64 long long //For convenience
static constexpr _int64 exp2_55 = 0x80000000000000ull; // (1 << 55) - used for checking that the msb is in bit 55

extern "C" {
   _internal
   inline unsigned int _fmaf_clz32(unsigned int _i) {
      unsigned int _n = 32;
      if (_i >= 0x00010000) {
         _i >>= 16;
         _n -= 16;
      }
      if (_i >= 0x00000100) {
         _i >>=  8;
         _n -=  8;
      }
      if (_i >= 0x00000010) {
         _i >>=  4;
         _n -=  4;
      }
      if (_i >= 0x00000004) {
         _i >>=  2;
         _n -=  2;
      }
      _n -= _i & ~(_i >> 1);
      return _n;
   }
   _internal
   inline unsigned int _fmaf_clz64(const unsigned _int64 _i) {
      const unsigned int _hi = _i >> 32,
                         _lo = _i & 0xffffffff;
      return _hi ? _fmaf_clz32(_hi) : (32 + _fmaf_clz32(_lo));
   }
}
extern "C"
   _internal
   float _ieee754_fmaf(const float _x, const float _y, const float _z) {
   _ieee754_f32 _fx(_x),
                _fy(_y),
                _fz(_z);
   //_x * _y == +-inf, and _z == -+inf
   if ((_fx._f_core._exp + _fy._f_core._exp >= 255 + flt_exp_bias) &&
      _fz._f_core._exp == 255 && (_fx._f_core._sgn ^ _fy._f_core._sgn) != _fz._f_core._sgn) { //Differing signs
      _fx._i = flt_qnan;
      return _fx._f;
      }
   //If _z is infinite and _x and _y are finite, return _z (not NaN)
   if (_fz._f_core._exp == 0xff &&
      _fx._f_core._exp != 0xff &&
      _fy._f_core._exp != 0xff)
      return _z;
   //If _z is zero and _x and _y are not, return _x * _y (No extended precision needed)
   //We leave out z due to possible incorrect signed zeros arising if _x * _y underflows.
   if (_z == 0.0f && _x != 0.0f && _y != 0.0f)
      return _x * _y;
   //If any of _x, _y, or _z is non-finite, or _x * _y is zero, simply return _x * _y + _z
   if (_fz._f_core._exp == 0xff ||
      _fx._f_core._exp == 0xff ||
      _fy._f_core._exp == 0xff ||
      _x == 0.0f || _y == 0.0f)
      return _x * _y + _z;
   //This is when fmaf() will certainly overflow (We can just return _x * _y)
   if (_fx._f_core._exp + _fy._f_core._exp > 0xff /*255*/ + flt_exp_bias)
      return _x * _y;

   //Actual math.

   int _x_exp = _fx._f_core._exp - 1,
       _y_exp = _fy._f_core._exp - 1,
       _z_exp = _fz._f_core._exp - 1;
   unsigned _int64 _x_mant = _fx._f_core._mantissa,
                   _y_mant = _fy._f_core._mantissa,
                   _z_mant = _fz._f_core._mantissa;
   unsigned int _x_sgn = _fx._f_core._sgn,
                _y_sgn = _fy._f_core._sgn,
                _z_sgn = _fz._f_core._sgn;
   //Subnormal checks for each _x, _y, and _z.
   //This is to get a normalized mantissa to work with so we can do
   //integer arithmetic on all of them
   if (_fx._f_core._exp == 0) {
      _fx._f *= 0x1p23f; //This is exact
      _x_mant = _fx._f_core._mantissa; //Mantissa is now normalized
      _x_exp = _fx._f_core._exp - 24;
   }
   if (_fy._f_core._exp == 0) {
      _fy._f *= 0x1p23f;
      _y_mant = _fy._f_core._mantissa;
      _y_exp = _fy._f_core._exp - 24;
   }
   if (_fz._f_core._exp == 0) {
      _fz._f *= 0x1p23f;
      _z_mant = _fz._f_core._mantissa;
      _z_exp = _fz._f_core._exp - 24;
   }
   _x_mant |= one_mant;
   _y_mant |= one_mant;
   // multiply _x * _y
    unsigned int _r_sgn = (_x_sgn ^ _y_sgn),
                 _m_sgn = _r_sgn;
    int _r_exp = _x_exp + _y_exp - 127 + 2;
   _y_mant <<= 8; //We shift this so the multiplication results in a 56-bit result, which is what we want.
   unsigned _int64 _r_mant = (_x_mant * _y_mant);
   if (!(_r_mant & exp2_55)) { //Implicit one is not in bit 55
      _r_mant <<= 1;
      _r_exp --;
   }
   /*
    * Now we have:
    * Bit 55: implicit 1
    * Bits 54 -> 32: Mantissa bits (23 of them)
    * Bits 31 -> 0: Round/sticky bits
    */
   // add _z to _x * _y
   unsigned int _round_sticky; //The multiplication was exact, but now we must track rounding.
   _z_mant |= one_mant;
   unsigned _int64 _z_mant_ext = _z_mant;
   _z_mant_ext <<= 32; //to get _z_mant as a 64-bit int, with msb in bit 55
   int _shift; // = _r_exp - _z_exp;
   unsigned _int64 _t; //Temp for rounding
   unsigned _int64 _large, _small;
   const int _rnd = c_get_f_round();
   //Sort (_x * _y) and _z by their magnitude
   if ((_r_exp > _z_exp) ||
      ((_r_exp == _z_exp) && (_r_mant > _z_mant_ext))) {
      //_r is larger than _z either in exponent or mantissa (if exponents are equal)
      _shift = _r_exp - _z_exp;
      _large = _r_mant;
      _small = _z_mant_ext;
   } else { //_z is larger than _r
      _shift = _z_exp - _r_exp;
      _r_exp = _z_exp;
      _r_sgn = _z_sgn;
      _large = _z_mant_ext;
      _small = _r_mant;
   }
   //Shift (denormalize) _small to get it ready for addition
   if (_shift == 0)
      _round_sticky = 0; //No precision loss when we add
   else if (_shift > 63) { //_small is swallowed
      //Only sticky matters now
      _round_sticky = 1;
      _small = 0;
   } else {
      // _small is partially visible, so we need to pay attention to it with rounding bits.
      _t = _small << (64 - _shift);
      _round_sticky = (_t >> 32) | !!(_t & 0xffffffff); //All bits that are lost with the shift
      //!!(_t & ...) ensures that we get all the sticky bits, even those in the least significant bits
      _small >>= _shift;
   }
   //Now, we can finally add/subtract, since all the hard work is done!
   if (_m_sgn != _z_sgn) { //Subtraction
      _large -= _small;
      _large -= !!_round_sticky; /*If we have extra lost bits,
      remove another bit from _large to truncate so rounding is accurate*/
      if (_large == 0) { //absolute cancellation, where we can safely return zero
         //fmaf(x, y, -xy) is +0.0f for all rounding modes except downard (toward -inf)
         return (_rnd == _round_down) ? -0.0f : 0.0f; //Signed zero subtraction
      }
      //We might need to renormalize the mantissa
      if (!(_large & exp2_55)) { //Implicit 'one' bit has moved out of bit 55!
         int _lz = _fmaf_clz64(_large);
         _shift = _lz - 8; //To shift over into bit 55
         _large <<= _shift;
         _r_exp -= _shift;
      }
   } else { //Addition
      _large += _small;
      //Normalize the mantissa
      if (_large & 0x100000000000000ull /*2 ^ 56 (i.e. _exp2_55 << 1*/) {
         //Check if the addition resulted in the implicit one in bit 56
         _t = _large << 63;
         _round_sticky = (_t >> 32) | !!_round_sticky;
         _large >>= 1;
         _r_exp ++;
      }
   }
   _r_mant = _large >> 32;
   /*
    *rounding/returns
    */
   _round_sticky = (_large & 0xffffffff) | !!_round_sticky;
   if (_r_exp + 1 >= 255) { //Overflow - either +-inf or +-(largest normal), depending on the rounding mode.
      //_r_exp was decremented in the beginning, so we increment again
      switch (_rnd) {
         case _round_nearest:
            _fx._i = flt_inf_bits | (_r_sgn << 31);
            break;
         case _round_up:
            _fx._i = _r_sgn ? 0xff7fffff /*-flt_max bits*/ : flt_inf_bits;
            break;
         case _round_down:
            _fx._i = _r_sgn ? 0xff800000 : 0x7f7fffff /*bit pattern for flt_max*/;
            break;
         default: //_round_zero
            _fx._i = 0x7f7fffff | (_r_sgn << 31); //+-flt_max
            break;
      }
      return _fx._f;
   }
   //Round result
   if (_r_exp >= 0) { //_r_exp was decremented in the beginning, so we do * >= 0 instead of > 0
      switch (_rnd) {
         case _round_nearest:
            _r_mant += (_round_sticky == 0x80000000 /*Only round bit is set - tie*/) ?
               (_r_mant & 1 /*Only add if mantissa is odd*/) : (_round_sticky >> 31);
            break;
         case _round_up:
            _r_mant += (_round_sticky && !_r_sgn); //Only add if positive and any round bits are set
            break;
         case _round_down:
            _r_mant += (_round_sticky && _r_sgn); //Similar to _round_up, but we add if negative
            break;
         default: //_round_zero
            break; //We do nothing because truncation is what we desire in this rounding mode.
      }
      //Reassemble the float
      _fx._i = _r_mant + (_r_exp << 23) + (_r_sgn << 31);
      //_fx._f_core._exp = _r_exp + 1;
      //_fx._f_core._sgn = _r_sgn;
      return _fx._f;
   }
   //Subnormal, zero, smallest normal
   /*
    * We shift the mantissa into place by shifting by -_r_exp, then calculate rounding
    * and then round depending on the current rounding mode.
    */
   _shift = -_r_exp;
   //if (_shift) //If _shift is zero, and we enter this case, we won't preserve any round bit
      _round_sticky = (_shift > 25) ? 1 : ((_r_mant << (32 - _shift)) | !!_round_sticky);
   _r_mant = (_shift > 25) ? 0 : (_r_mant >> _shift);
   switch (_rnd) {
      case _round_nearest:
         _r_mant += (_round_sticky == 0x80000000 /*Only round bit is set - tie*/) ?
            (_r_mant & 1 /*Only add if mantissa is odd*/) : (_round_sticky >> 31);
         break;
      case _round_up:
         _r_mant += (_round_sticky && !_r_sgn); //Only add if positive and any round bits are set
         break;
      case _round_down:
         _r_mant += (_round_sticky && _r_sgn); //Similar to _round_up, but we add if negative
         break;
      default: //_round_zero
         break; //We do nothing because truncation is what we desire in this rounding mode.
   }
   _fx._f_core._mantissa = _r_mant;
   //skip exp, or leave it zero
   _fx._f_core._exp = 0;
   _fx._f_core._sgn = _r_sgn;
   _fx._i = _r_mant + (_r_sgn << 31);
   return _fx._f;
} // _ieee754_fmaf

extern "C"
	_internal
   _deprecated("11/07/25", "This method is sometimes not correct. Use the int math method instead.")
    float _dekker_fmaf(const float _x, const float _y, const float _z) {
	float _ix = _x, _iy = _y, _iz = _z; //For actual work
	//return _x * _y + _z;

	_ieee754_f32
		_fx(_x),
		_fy(_y),
		_fz(_z);
	int _scale = 0;
	//We can skip all the hard scaling work if these conditions are not met (i.e. the floats are safe to work with)
	if (_fx._f_core._exp + _fy._f_core._exp >= 0xff + flt_exp_bias - flt_mant_len || //_x * _y will overflow(either now or in scaling)
		_fx._f_core._exp >= 0xff - flt_mant_len || //_x is very large
		_fy._f_core._exp >= 0xff - flt_mant_len || //_y is very large
		_fz._f_core._exp >= 0xff - flt_mant_len || //_z is very large
		_fx._f_core._exp + _fy._f_core._exp <= flt_exp_bias + flt_mant_len) { //_x * _y is very small (Might underflow to zero)
		//I don't even think it is possible to practically makes these czechs branchless...

		//I don't know why like half of these work because I stole them all from glibc's implementation...

		//If _z is infinite and _x and _y are finite, return _z (not NaN)
		if (_fz._f_core._exp == 0xff &&
			_fx._f_core._exp != 0xff &&
			_fy._f_core._exp != 0xff)
			return _z;
		//If _z is zero and _x and _y are not, return _x * _y (No extended precision needed)
		//We leave out z due to possible incorrect signed zeros arising if _x * _y underflows.
		if (_z == 0.0f && _x != 0.0f && _y != 0.0f)
			return _x * _y;
		//If any of _x, _y, or _z is non-finite, or _x * _y is zero, simply return _x * _y + _z
		if (_fz._f_core._exp == 0xff ||
			_fx._f_core._exp == 0xff ||
			_fy._f_core._exp == 0xff ||
			_x == 0.0f || _y == 0.0f)
			return _x * _y + _z;
		//This is when fmaf() will certainly overflow (We can just return _x * _y)
		if (_fx._f_core._exp + _fy._f_core._exp > 0xff /*255*/ + flt_exp_bias)
			return _x * _y;

		//This results in an exponent of -25 (- 127) for _x * _y, so 1/4 of flt_sub_epsilon.
	   //We manually add the underflow flags if we need to
		//Now, only the sign of the result matters...
		if (_fx._f_core._exp + _fy._f_core._exp < (flt_exp_bias - flt_mant_len - 2)) {
			const bool _n = _fx._f_core._sgn ^ _fy._f_core._sgn;
			const float _tiny = _n? -flt_sub_epsilon : flt_sub_epsilon;
			if (_fz._f_core._exp >= 3) //Numbers larger than or as large as this will not be affected by adding _tiny
				return _tiny + _z;
			//Scale up _z, then add _tiny before scaling down (Forces inexact and underflow)
			_fz._f = _z * 0x1p25f + _tiny;
			if (_fy._f_core._exp >= 26) { //Assuming tininess after rounding (because I'm lazy). The number 26 comes from deep magic (
				//If the exponent is less than this the result could underflow)
				const float _force_underflow = _x * _y;
				math_f_eval(_force_underflow); //Force the compiler to recognize this value to set the underflow flag
			}
			return _fz._f * 0x1p-25f;
		}

		if (_fx._f_core._exp + _fy._f_core._exp >= 0xff + flt_exp_bias - flt_mant_len) {
			// _x * _y Dangerously close to overflowing
			//Compute the result * 2^-23 and scale back afterward
			if (_fx._f_core._exp > _fy._f_core._exp)
				_fx._f_core._exp -= flt_mant_len;
			else
				_fy._f_core._exp -= flt_mant_len;
			//If _z's exponent is small we need not scale it down
			//This is because if _z is too small then it's so tiny anyway that it gets swallowed by _x * _y
			//So we don't need to scale it down because that's useless
			if (_fz._f_core._exp > flt_mant_len)
				_fz._f_core._exp -= flt_mant_len;
			_scale = 1;
		}
		else if (_fz._f_core._exp >= 0xff - flt_mant_len) { //_z is very large
			//If _x and _y exponents are rather small
			//We adjust either _x's or _y's exponents to help
			if (_fx._f_core._exp + _fy._f_core._exp <=
				flt_exp_bias + 2 * flt_mant_len) { //Increase exponent of _x or _y so it is not swallowed by _z
				if (_fx._f_core._exp > _fy._f_core._exp)
					_fx._f_core._exp += 2 * flt_mant_len + 2;
				else
					_fy._f_core._exp += 2 * flt_mant_len + 2;
				}
			else if (_fx._f_core._exp > _fy._f_core._exp) {
				if (_fx._f_core._exp > flt_mant_len)
					_fx._f_core._exp -= flt_mant_len;
			}
			else if (_fy._f_core._exp > flt_mant_len)
				_fy._f_core._exp -= flt_mant_len;
			//Scale down _z in the same way to maintain accuracy with computations
				_fz._f_core._exp -= flt_mant_len;
			_scale = 1;
		}
		else if (_fx._f_core._exp >= 0xff - flt_mant_len) { //_x only is very large
			//This can be fixed easily by scaling down _x and scaling up _y
			_fx._f_core._exp -= flt_mant_len;
			if (_fy._f_core._exp) //_y is normal
				_fy._f_core._exp += flt_mant_len;
			else
				_fy._f *= 0x1p+23;
		}
		else if (_fy._f_core._exp >= 0xff - flt_mant_len) { //_y only is very large
			//This can be fixed easily by scaling down _y and scaling up _x
			//Mirror of previous case
			_fy._f_core._exp -= flt_mant_len;
			if (_fx._f_core._exp) //_x is normal
				_fx._f_core._exp += flt_mant_len;
			else
				_fx._f *= 0x1p+23;
		}
		else if (_fx._f_core._exp + _fy._f_core._exp <= flt_exp_bias + flt_mant_len) {
			if (_fx._f_core._exp > _fy._f_core._exp)
				_fx._f_core._exp += 2 * flt_mant_len + 2; //48
			else
				_fy._f_core._exp += 2 * flt_mant_len + 2; //48
			if (_fz._f_core._exp <= 4 * flt_mant_len + 6) { //94 - Probably just heuristic
															 //IDK I stole it from glibc anyway
				if (_fz._f_core._exp) //_z is normal
					_fz._f_core._exp += 2 * flt_mant_len + 2; //48
				else
					_fz._f *= 0x1p+48;
				_scale = -1;
			}
			//_scale = -1;
			//Otherwise _x * _y is fine
			//Everything is fine* *_x, _y, and _z are all relatively small
		}
		//Redefine parameters to be scaled versions
		_ix = _fx._f;
		_iy = _fy._f;
		_iz = _fz._f;
}
	if ((_ix == 0.0f || _iy == 0.0f) && _iz == 0.0f) {
		_ix = math_opt_barrier(_x);
		return _ix * _iy + _iz;
	}

	//Multiplication of _rh + _rl = _x * _y through Dekker's multiplying algorithm
#define _c 4097.0f // Constant used for Dekker splitting.
				// = ((1 << (flt_mant_len + 1) / 2) + 1)
	//Apparently 13 does not work!
	float _xh = _ix * _c;
	float _yh = _iy * _c;
#undef _c
	float _rh = _ix * _iy; //High part of the product

	_xh = (_ix - _xh) + _xh;
	_yh = (_iy - _yh) + _yh;
	const float _xl = _ix - _xh;
	const float _yl = _iy - _yh;
	const float _rl = (((_xh * _yh - _rh) + _xh * _yl) + _xl * _yh) + _xl * _yl; //Low part (error) of the product
	//Addition of _ah + _al = _rh + _z through Knuth's adding algorithm
	float _ah = _iz + _rh, //High part of sum
		  _th = _ah - _iz, //Intermediate terms _th and _tl
		  _tl = _ah - _th; //^
	_th = _rh - _th;
	_tl = _iz - _tl;
	const float _al = _th + _tl; //Low part (error) of sum

	//If the result is an exact zero (Sum and product error - sum error will always be zero if the sum is)
   //Ensure correct sign.
	if (_ah == 0.0f && _rl == 0.0f) {
		math_opt_barrier(_iz);
		return _iz + _rh;
	}

	//const float _err = _al + _rl; //Total combined error of the product and sum

	//Currently the error is between 0-0.5ulp (Which rounds to 1ulp)
	//Now we round to odd

   _fx._f = _al + _rl; //Total error of the naive multiply-add, which we can add on later.
   int _inexact = (_fx._f != 0.0f);

	if (_scale == -1) {

	 	if ((_fy._f_core._mantissa & 1) == 0) //Mantissa is even - round to odd
	 		_fy._f_core._mantissa |= _inexact; //Round to odd
	   _fy._f = _ah + _fx._f;
		math_f_eval(_fy._f); //Evaluate it now
	}

	_ah = math_opt_barrier(_ah);

	if (_scale == 0) {
		if ((_fx._f_core._mantissa & 1) == 0 && _fx._f_core._exp != 0xff) //Finite, even-mantissa float
			_fx._f_core._mantissa |= (_inexact);
		return _fx._f + _ah;
	}
	if (_scale == 1) {
		if ((_fx._f_core._mantissa & 1) == 0 && _fx._f_core._exp != 0xff) //Finite, even-mantissa float
			_fx._f_core._mantissa |= _inexact;
		return (_fx._f + _ah) * 0x1p23f;
	}
	else {
		//Check if _ah + _fx._f is exact (Then the rounding happens only when scaling down)
		if (!_inexact) {
			return _fy._f * 0x1p-48f;
		}
		//If result rounded to zero is normal (No double rounding happens)
		if (_fy._f_core._exp > 48) {
			//If exponent is large enough we can scale down safely
			return (_ah + _fx._f) * 0x1p-48f;
		}
		if (_fy._f_core._exp == 48) { //_fz._f is subnormal (Because we scaled by 0x1p48f before)
	      _fz._f = _ah + _fx._f;
			if (_fz._f_core._exp == 49) {
				return _fz._f * 0x1p-49f;
			}
			//Actual magic
			//If _fz._f is subnormal
			_fz._f = 0.0f;
			_fz._f_core._mantissa = ((_fy._f_core._mantissa & 3) << 1) | _inexact;
			_fz._f_core._sgn = _fy._f_core._sgn;
			_fy._f_core._mantissa &= ~3u;
			_fy._f *= 0x1p-48f;
			_fz._f *= 0x1p-2f;
			return _fy._f + _fz._f;
		}
		_fy._f_core._mantissa |= _inexact;
		return _fy._f * 0x1p-48f;
	}
} //_dekker_fmaf

extern "C" //This must go in extern "C" to avoid name mangling
	//Format it nicely for the public API
	_strong_alias(c_fmaf, _ieee754_fmaf);

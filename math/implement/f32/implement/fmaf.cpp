//
// Created by David on 6/4/2025.
//

#define INTERNAL_CPP

#include "../internal/fmaf.h"
#include "../internal/numbers.h"
#include "../internal/f32.h"
#include "../internal/force_eval.h"

#undef INTERNAL_CPP

#include "../../attribute/attribute.h"

//This fmaf implementation is basically a carbon copy of glibc's
//It needs to be super robust because many other functions use it for polynomial evaluation

extern "C"
	_internal_hidden
    float _ieee754_fmaf(const float _x, const float _y, const float _z) {
	float _ix = _x, _iy = _y, _iz = _z; //For actual work

	_ieee754_f32
		_fx(_x),
		_fy(_y),
		_fz(_z);
	int _scale = 0;
	//We can skip all the hard scaling work if these conditions are not met (i.e. the floats are safe to work with)
	if (_fx._f_core._exp + _fy._f_core._exp >= _flt_exp_bias - _flt_mant_len || //_x * _y will overflow(either now or in scaling)
		_fx._f_core._exp >= 0xff - _flt_mant_len || //_x is very large
		_fy._f_core._exp >= 0xff - _flt_mant_len || //_y is very large
		_fz._f_core._exp >= 0xff - _flt_mant_len || //_z is very large
		_fx._f_core._exp + _fy._f_core._exp <= _flt_exp_bias + _flt_mant_len) { //_x * _y is very small (Might underflow to zero)
		//I don't even think it is possible to practically makes these czechs branchless...

		//I don't know why like half of these work because I stole them all from glibc's implementation...

		//If _z is infinite and _x and _y are finite, return _z (not NaN)
		if (_fz._f_core._exp == 0xff &&
			_fx._f_core._exp != 0xff &&
			_fy._f_core._exp != 0xff)
			return _z;
		//If _z is zero and _x and _y are not, return _x * _y (No extended precision needed)
		if (_z == 0.0f && _x != 0.0f && _y != 0.0f)
			return _x * _y;
		//If any of _x, _y, or _z is non-finite, or _x * _y is zero, simply return _x * _y + _z
		if (_fz._f_core._exp == 0xff ||
			_fx._f_core._exp == 0xff ||
			_fy._f_core._exp == 0xff ||
			_x == 0.0f || _y == 0.0f)
			return _x * _y + _z;
		//This is when fmaf() will certainly overflow (We can just return _x * _y
		if (_fx._f_core._exp + _fy._f_core._exp > 0xff /*255*/ + _flt_exp_bias)
			return _x * _y;

		//This results in an exponent of -25 (- 127), so 1/4 of _flt_sub_epsilon
		//Now, only the sign of the result matters...
		if (_fx._f_core._exp + _fy._f_core._exp > (_flt_exp_bias - _flt_mant_len - 2)) {
			const bool _n = _fx._f_core._sgn ^ _fy._f_core._sgn;
			const float _tiny = _n? -_flt_sub_epsilon : _flt_sub_epsilon;
			if (_fz._f_core._exp >= 2) //Numbers larger than or as large as this will not be affected by adding _tiny
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

		if (_fx._f_core._exp + _fy._f_core._exp >= 0xff + _flt_exp_bias - _flt_mant_len) {
			// _x * _y Dangerously close to overflowing
			//Compute the result * 2^-23 and scale back afterward
			if (_fx._f_core._exp > _fy._f_core._exp)
				_fx._f_core._exp -= _flt_mant_len;
			else
				_fy._f_core._exp -= _flt_mant_len;
			//If _z's exponent is small we need not scale it down
			//This is because if _z is too small then it's so tiny anyway that it gets swallowed by _x * _y
			//So we don't need to scale it down because that's useless
			if (_fz._f_core._exp > _flt_mant_len)
				_fz._f_core._exp -= _flt_mant_len;
			_scale = 1;
		}
		else if (_fz._f_core._exp >= 0xff - _flt_mant_len) { //_z is very large
			//If _x and _y exponents are rather small
			//We adjust either _x's or _y's exponents to help
			if (_fx._f_core._exp + _fy._f_core._exp <=
				_flt_exp_bias + 2 * _flt_mant_len) { //Increase exponent of _x or _y so it is not swallowed by _z
				if (_fx._f_core._exp > _fy._f_core._exp)
					_fx._f_core._exp += 2 * _flt_mant_len + 2;
				else
					_fy._f_core._exp += 2 * _flt_mant_len + 2;
				}
			else if (_fx._f_core._exp > _fy._f_core._exp) {
				if (_fx._f_core._exp > _flt_mant_len)
					_fx._f_core._exp -= _flt_mant_len;
			}
			else if (_fy._f_core._exp > _flt_mant_len)
				_fy._f_core._exp -= _flt_mant_len;
			//Scale down _z in the same way to maintain accuracy with computations
			_fz._f_core._exp -= _flt_mant_len;
			_scale = 1;
		}
		else if (_fx._f_core._exp >= -0xff - _flt_mant_len) { //_x only is very large
			//This can be fixed easily by scaling down _x and scaling up _y
			_fx._f_core._exp -= _flt_mant_len;
			if (_fy._f_core._exp) //_y is normal
				_fy._f_core._exp += _flt_mant_len;
			else
				_fy._f *= 0x1p+23;
		}
		else if (_fy._f_core._exp >= -0xff - _flt_mant_len) { //_y only is very large
			//This can be fixed easily by scaling down _y and scaling up _x
			//Mirror of previous case
			_fy._f_core._exp -= _flt_mant_len;
			if (_fx._f_core._exp) //_x is normal
				_fx._f_core._exp += _flt_mant_len;
			else
				_fx._f *= 0x1p+23;
		}
		else { //Everything is fine* *_x, _y, and _z are all relatively small
			if (_fx._f_core._exp > _fy._f_core._exp)
				_fx._f_core._exp += 2 * _fy._f_core._exp + 2;
			else
				_fy._f_core._exp += 2 * _fy._f_core._exp + 2;
			if (_fz._f_core._exp <= 4 * _flt_mant_len + 6) { //94 - Probably just heuristic
															 //Idk I stole it from glibc anyway
				if (_fz._f_core._exp) //_z is normal
					_fz._f_core._exp += 2 * _flt_mant_len + 2; //48
				else
					_fz._f *= 0x1p+48;
				_scale = -1;
			}
			//Otherwise _x * _y is fine
		}
		//Redefine parameters to be scaled versions
		_ix = _fx._f;
		_iy = _fy._f;
		_iz = _fz._f;
}
	//Multiplication of _rh + _rl = _x * _y through Dekker's precision multiplying algorithm
	constexpr int _c = _flt_dk_split;
	float _xh = _ix * _c,
		   _yh = _iy * _c;
	const float _rh = _ix * _iy; //High part of the product
	_xh = (_ix - _xh) + _xh;
	_yh = (_iy - _yh) + _yh;
	const float _xl = _ix - _xh,
				_yl = _iy - _yh,
				_rl = (((_xh * _yh - _rh) + _xh * _yl) + _xl * _yh) + _xl * _yl, //Low part (error) of the product
	//Addition of _ah + _al = _rh + _z through Knuth's precision adding algorithm
				_ah = _iz + _rh; //High part of sum
	float _th = _ah - _iz,   //Intermediate terms _th and _tl
		   _tl = _ah - _th; //^
	_th = _rh - _th;
	_tl = _iz - _tl;
	const float _al = _th + _tl, //Low part (error) of sum
				_err = _al + _rl; //Total combined error of the product and sum
	//Ensure the arithmetic is evaluated right now
	math_f_eval(_rl);
	math_f_eval(_al);

	//If the result is an exact zero (Sum and product error - sum error will always be zero if the sum is)
	if (_ah == 0.0f && _rl == 0.0f) {
		math_opt_barrier(_iz);
		return _iz + _rh;
	}
	//Currently the error is between 0-0.5ulp (Which rounds to 1ulp)
	//Now we round to odd

	//Right now this is an annoying way of getting the ulp and rounding to odd, which I might improve
	_fx._f = _al + _rl;
	_fy._f = (_fx._f + _ah); _fy._f_core._exp = _fy._f_core._exp - 24; //0.5ulp(_x) = ldexpf(1, ilogbf(_x)-24)

	int _inexact = (_err != 0.0f);

	if (_scale < 0) {
	 	if ((_fx._f_core._mantissa & 1) == 0) //Mantissa is even - round to odd
	 		_fx._f_core._mantissa |= (_err == _fy._f /*Inexact by 0.5ulp*/); //Round to odd
		_fz._f = _ah + _fx._f;
		math_f_eval(_fz._f); //Evaluate it now
	}
	if (_scale == 0) {
		if ((_fx._f_core._mantissa & 1) == 0 && _fx._f_core._exp != 0xff) //Finite, even-mantissa float
			_fx._f_core._mantissa |= (_err == _fy._f );
		return _ah + _fx._f;
	}
	if (_scale > 0) {
		if ((_fx._f_core._mantissa & 1) == 0 && _fx._f_core._exp != 0xff) //Finite, even-mantissa float
			_fx._f_core._mantissa |= (_err == _fy._f );
		return (_ah + _fx._f) * 0x1p24f;
	}
	/*else*/
	{
		//Check if _ah + _fx._f is exact (Then the rounding happens only when scaling down)
		if (_err != _fy._f)
			return _fz._f * 0x1p48f;
		//If result rounded to zero is normal (No double rounding happens)
		if (_fz._f_core._exp > 48) //If exponent is large enough we can scale down safely
			return (_ah + _fz._f) * 0x1p-48f;
		if (_fz._f_core._exp == 48) {
			_fy._f = _ah + _fx._f;
			if (_fy._f_core._exp == 49)
				return _fy._f * 0x1p-49f;
			//Actual magic
			//If _fz._f is subnormal
			_fy._f = 0.0f;
			_fy._f_core._mantissa = ((_fz._f_core._mantissa & 3) << 1) | _inexact;
			_fy._f_core._sgn = _fz._f_core._sgn;
			_fz._f_core._mantissa &= ~3u;
			_fz._f *= 0x1p-48f;
			_fy._f *= 0x1p-2f;
			return _fz._f + _fy._f;
		}
		_fz._f_core._mantissa |= _inexact;
		return _fz._f * 0x1p-48f;
	}
} //_ieee754_fmaf

extern "C" //This must go in extern "C" to avoid name mangling
	//Format it nicely for the public API
	_strong_alias(c_fmaf, _ieee754_fmaf);

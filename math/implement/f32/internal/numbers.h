//
// Created by David on 6/7/2025.
//

#ifndef NUMBERS_H
//Include guard
#define NUMBERS_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include numbers.h directly. Use "c_math.h" instead.
#endif

//THIS FILE IS FOR F32 ONLY (There is another one in implement/f64)
//Like the rest of this library, everything assumes the IEEE754 Standard

/*
 * Mathematical constants
 */

///The mathematical constant e as a 32-bit floating-point number.
///e is Euler's number, the base of the natural logarithm and exponential function.
inline constexpr float math_e_f = 2.71828174591064453125f;
// ///The number e as a 64-bit floating-point number.
// ///e is Euler's number, the base of the natural logarithm.
// inline constexpr float _math_e = 2.7182818284590450908;

///The base-2 logarithm of Euler's number, e, as a 32-bit floating-point number.
inline constexpr float math_log2_e_f = 1.44269502162933349609f;
// //The base-2 logarithm of Euler's number, e, as a 64-bit floating-point number
// inline constexpr float _log2_e = 1.442695040888963387;

///The base-10 logarithm of Euler's number, e, as a 32-bit floating-point number.
inline constexpr float math_log10_e_f = 0.434294492006301879883f;

///The mathematical constant π (pi) as a 32-bit floating-point number.
inline constexpr float math_pi_f = 3.14159274101257324219f;

///1/π (pi) as a 32-bit floating-point number.
inline constexpr float math_inv_pi_f = 0.318309873342514038086f;

///1/√π (1 / The square root of pi) as a 32-bit floating-point number.
inline constexpr float math_inv_sqrt_pi_f = 0.564189612865447998047f;

///The natural logarithm of 2 as a 32-bit floating-point number.
inline constexpr float math_log_2_f = 0.693147182464599609375f;

///The natural logarithm of 10 as a 32-bit floating-point number.
inline constexpr float math_log_10_f = 2.30258512496948242188f;

///√2 (The square root of 2) as a 32-bit floating-point number.
inline constexpr float math_sqrt_2_f = 1.41421353816986083984f;
///√3 (The square root of 3) as a 32-bit floating-point number.
inline constexpr float math_sqrt_3_f = 1.73205077648162841797f;

///1/√3 (1 / The square root of 3) as a 32-bit floating-point number.
inline constexpr float math_inv_sqrt_3_f = 0.577350258827209472656f;

///Not Euler's number, but Euler's Constant (aka. the Euler Mascheroni constant) as a 32-bit floating-point number.
///Often denoted as γ (Gamma).
inline constexpr float math_egamma_f = 0.577215671539306640625f;

///φ (Phi), the Golden Ratio, defined as (1 + sqrt(5)) / 2.
inline constexpr float math_phi_f = 1.61803400516510009766f;

/*
 * IEEE754 floating-point standard constants
 */

///A binary mask for the sign bit of a 32-bit floating point number.
inline constexpr unsigned int flt_sgn_mask = 0x80000000;
///A binary mask for the exponent bits of a 32-bit floating point number.
inline constexpr unsigned int flt_exp_mask = 0x7f800000;
///A binary mask for the mantissa bits of a 32-bit floating-point number.
inline constexpr unsigned int flt_mant_mask = 0x007fffff;

///A binary mask for the exponent and mantissa bits of a 32-bit floating-point number.
///(Sets the sign bit to zer0, effective absolute value).
inline constexpr unsigned int flt_abs_mask = 0x7fffffff;

///The binary representation of a 32-bit floating-point infinity.
inline constexpr unsigned int flt_inf_bits = 0x7f800000;

///The binary representation of a 32-bit floating-point quiet NaN
///(Not a Number, propagating).
inline constexpr unsigned int flt_qnan = 0x7fc00000;
///The binary representation of a 32-bit floating-point signaling NaN
///(Not a Number, not propagating on certain architectures).
inline constexpr unsigned int flt_snan = 0x7fa00000;

///The length in bits of the mantissa of a 32-bit floating-point number.
inline constexpr unsigned int flt_mant_len = 0x017;
///The length in bits of the exponent of a 32-bit floating-point number.
inline constexpr unsigned int flt_exp_len = 0x08;
///The length in bits of the sign of a 32-bit floating-point number (Always 1).
inline constexpr unsigned int flt_sgn_len = 1;

///The inherent exponent bias of a 32-bit floating-point number
///imposed by the IEEE754 standard.
inline constexpr unsigned int flt_exp_bias = 0x07f;

/*
 * Constants related to floating-point and used in some functions
 */

///The bit pattern of the largest possible non-integral value in the 32-bit floating-point format.
///Basically only used for truncf() and other rounding functions
inline constexpr unsigned int flt_max_dec_bits = 0x4affffff;

///The largest possible non-integral value in the 32-bit floating-point format.
inline constexpr float flt_max_dec = 8388607.5f;

///The distance to the next representable value after 1.0 in the 32-bit floating-point format.
inline constexpr float flt_epsilon = 1.1920928955078125e-7f;

///Absolute epsilon - The smallest (normal) 32-bit floating-point number.
inline constexpr float flt_a_epsilon = 1.17549435082228750797e-38f;

///Subnormal epsilon - The smallest subnormal 32-bit floating-point number.
///It is the smallest representable positive 32-bit floating-point number that is not zero
inline constexpr float flt_sub_epsilon = 1.40129846432481707092e-45f;

///The largest representable finite number in the 32-bit floating-point format.
inline constexpr float flt_max = 3.40282346638528859812e+38f;

///For Dekker Splitting in for 32-bit floats ((23 / 2) + 1)
inline constexpr unsigned int flt_dk_split = 13;

#endif //NUMBERS_H

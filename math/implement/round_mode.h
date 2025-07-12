//
// Created by David on 6/16/2025.
//

#ifndef ROUND_MODE_H
//Include guard
#define ROUND_MODE_H

/*
 * | The Lore |
 *
 * Auyuittuq Nasjonalpark er en nasjonalpark i det nordøstlige Canada.
 * Det er på polarsirkelen, fordi parken ligger ~67°53′N. Men det er ikke
 * den nordligste nasjonalparken i Canada, fordi det er Quttinirpaaq
 * Nasjonalpark, som ligger ~82°N, veldig langt over polarsirkelen! Det
 * ligger på Baffinøya, den største øya i Canada, som er en av de mest
 * fantastisk øyene i verden!
 *
 * Du skulle tro at nasjonalparken er bare omtrent 7/9 av veien nord, men
 * sinus til 67° er omtrent 0,9205, som er faktisk 90% av veien til
 * nordpolen! Men, Quttinirpaaq Nasjonalpark, hvis vi sier det samme, er
 * omtrent 0,99026 / 1.0 av veien til nordpolen!
 *
 * Auyuittuq Nasjonalpark har mange fjell. Thor Peak er et fint fjell
 * med en klipp som er 1,200m høy, verdens høyeste klippe! Det er også
 * Mount Asgard, Mount Odin, og mange flere.
 *
 * Folk som bor i nærheten av Auyuittuq Nasjonalpark vet mye... mange av
 * dem er fra det Far Realm, men inuittstammene vet ting generelt.
 */

// Rounding mode detection functions modified from the LLVM project
//https://github.com/llvm/llvm-project/blob/main/libc/src/__support/FPUtil/rounding_mode.h

//Check if the rounding mode == FE_UPWARD
// 1.0f + 2^-25 = 1.0f        for FE_TONEAREST, FE_DOWNWARD, and FE_TOWARDZERO,
//              = 0x1.000002f for FE_UPWARD.
inline bool is_round_up() {
    static volatile float _f = 0x1.0p-25f;
    return(1.0f + _f != 1.0f);
}

//Check if the rounding mode == FE_DOWNWARD
// -1.0f - 2^-25 = -1.0f        for FE_TONEAREST, FE_UPWARD, and FE_TOWARDZERO,
//               = -0x1.000002f for FE_DOWNWARD.
inline bool is_round_down() {
    static volatile float _f = 0x1.0p-25f;
    return(-1.0f - _f != -1.0f);
}

//Check if the rounding mode == FE_TONEAREST
// 1.5f + 2^-24 = 1.5f           for FE_TONEAREST, FE_DOWNWARD, and FE_TOWARDZERO,
//              = 0x1.100002p0f  for FE_UPWARD.
// 1.5f - 2^-24 = 1.5f           for FE_TONEAREST and FE_UPWARD,
//              = 0x1.0ffffep-1f for FE_DOWNWARD and FE_TOWARDZERO.
//If both of these conditions == 1.5f, the only common rounding mode is FE_TONEAREST
inline bool is_round_to_nearest() {
    static volatile float _f = 0x1.0p-24f;
    const float _y = _f;
    return(1.5f + _y == 1.5f - _y); // Both must be 1.5f
                                    //(Such a rounding mode where this is false does not exist)
}

//Check if the rounding mode == FE_TOWARDZERO
// 1.0f + 2^-23 + 2^-24 = 0x1.000002p0f  for FE_DOWNWARD and FE_TOWARDZERO,
//                      = 0x1.000004p0f  for FE_TONEAREST and FE_UPWARD.
// -1.0f - 2^-24        = -1.0f          for FE_TONEAREST, FE_U_WARD, and FE_TOWARDZERO,
//                      = -0x1.000002pof for FE_DOWNWARD.
//Therefore:
//  (0x1.000002p0f + 2^-24) + (-1.0f - 2^-24) = 2^-23 for FE_TOWARDZERO,
//                                            = 2^-22 for FE_TONEAREST and FE_UPWARD,
//                                            = 0 for FE_DOWNWARD.
inline bool is_round_to_zero() {
    static volatile float _f = 0x1.0p-24f;
    const float _y = _f;
    return((0x1.000002p0f + _y)
            + (-1.0f - _y)      // = -1.0f in FE_TOWARDZERO
            == 0x1.0p-23f);
}

#define _round_up      0x001
#define _round_down    0x002
#define _round_nearest 0x004
#define _round_zero    0x008

#define _rounding_mode int

inline _rounding_mode c_get_f_round() {
    static volatile float _f = 0x1.0p-24f;
    const float _y = _f,
                _z = (0x1.000002p0f + _y) + (-1.0f - _y);
    if (_z == 0.0f)
        return _round_down;
    if (_z == 0x1.0p-23f)
        return _round_zero;
    return(2.0f + _y == 2.0f) ? _round_nearest : _round_up;
}

#endif //ROUND_MODE_H
//
// Created by David on 6/8/2025.
//

#ifndef FORCE_EVAL_H
//Include Guard
#define FORCE_EVAL_H

#if !defined(MATH_INCLUDE) && !defined(INTERNAL_CPP)
        //Stop people from directly including this file unless they Know how...
        #error Do not include fabsf.h directly. Use "c_math.h" instead.
#endif

///Force the evaluation of _x, even if it looks useless.
///This is useful for fmaf() and other functions where rounding and flags are necessary
///@param _x The expression to be evaluated
inline void math_f_eval(const float _x) {
    const volatile float _y = _x; //Magically forces the compiler to pay attention to
                                  //the parameter instead of ignoring it
    (void) _y; //Silence the compiler
}

///Force the evaluation of _x without any optimizations from the compiler
///This is useful for functions where rounding and detail preservation are necessary
///@param _x The expression to be evaluated without optimizations
inline float math_opt_barrier(const float _x) {
    const volatile float _y = _x; //Orders the compiler not to optimize this into the void
    return _y;
}
#endif //FORCE_EVAL_H
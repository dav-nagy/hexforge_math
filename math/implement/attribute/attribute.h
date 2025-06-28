//
// Created by David on 6/2/2025.
//

#ifndef ATTRIBUTE_H
//Include guard
#define ATTRIBUTE_H

//This _strong_alias does not account for name mangling. Always wrap it with extern "C".
#define _strong_alias(_an, _n) \
    __typeof(_n) _an __attribute__(( alias(#_n) ))
//This _weak_alias does not account for name mangling. Always wrap it with extern "C".
#define _weak_alias(_an, _n) \
    __typeof(_n) _an __attribute__(( weak, alias(#_n) ))
//This library is for C++ ONLY (So, no need to #ifdef __cplusplus)

#ifdef __ELF__ //Linux and other ELF systems
    //This _internal does not account for name mangling. Always wrap it with extern "C".
    #define _internal \
        __attribute__((visibility("hidden")))
    #define _export //Useless
#else //For Windows
    #define _internal \
        //This _strong_alias does not account for name mangling. Always wrap it with extern "C".
        __declspec(dllexport)
//^^^Might be useless...

    #define _export \
        __attribute((__visibility__("default")))
#endif

// #define _deprecated \
//     __attribute__((deprecated))
#define _deprecated(since, msg) \
    __attribute__((deprecated("Since " since " — " msg)))



#endif //ATTRIBUTE_H

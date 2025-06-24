//
// Created by David on 6/2/2025.
//

#ifndef ATTRIBUTE_H
//Include guard
#define ATTRIBUTE_H

//Let the linker know that symbol _n is equivalent to _an
//(You likely will not know unless you are delving into this source code)

#define _strong_alias(_an, _n) \
    extern "C" __typeof(_n) _an __attribute__(( alias(#_n) ))
#define _weak_alias(_an, _n) \
    extern "C" __typeof(_n) _an __attribute__(( weak, alias(#_n) ))
//This library is for C++ ONLY (So, no need to #ifdef __cplusplus)

#ifdef __ELF__ //Linux and other ELF systems
    #define _internal \
    extern "C" __attribute__((visibility("hidden")))
    #define _export //Useless
#else //For Windows
    #define _internal \
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

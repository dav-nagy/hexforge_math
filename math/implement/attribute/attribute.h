//
// Created by David on 6/2/2025.
//

#ifndef ATTRIBUTE_H
//Include guard
#define ATTRIBUTE_H

//Let the linker know that symbol _n is equivalent to _an (You likely will not know)

#define _strong_alias(_an, _n) \
    extern "C" __typeof(_n) _an __attribute__((alias(#_n)))
#define _weak_alias(_an, _n) \
    extern "C" __typeof(_n) _an __attribute__((weak, alias(#_n)))
//This library is for C++ ONLY (So, no need to #ifdef __cplusplus)

#if defined(__ELF__) //Linux and other ELF systems
    #define _internal_hidden \
    extern "C" __attribute__((visibility("hidden")))
    #define _export //Useless
#else //For Windows
    #define _internal_hidden \
        __declspec(dllexport)
    #define _export \
        __attribute((__visibility__("default")))
#endif

#endif //ATTRIBUTE_H

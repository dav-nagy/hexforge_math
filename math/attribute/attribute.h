//
// Created by David on 6/2/2025.
//

#ifndef ATTRIBUTE_H
//Include guard
#define ATTRIBUTE_H

//Let the linker know that symbol _n is equivalent to _an (You likely will not know)
#define _strong_alias(_an, _n)                              \
    extern "C" __typeof(_n) _an __attribute__((__alias__(#_n)))
//This library is for C++ ONLY (So, no need to #ifdef __cplusplus)

#if defined(__GNUC__) || defined(__clang__) //I, in fact am. I might just remove this condition since it is personal library, but it can stay for now
    #define _internal_hidden \
    extern "C" __attribute__((visibility("hidden")))
#else
    #define _internal_hidden //Useless
#endif

#endif //ATTRIBUTE_H

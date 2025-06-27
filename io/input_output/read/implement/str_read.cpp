//
// Created by David on 6/2/2025.
//

#include "../str_read.h"
#include "../../ops/cio_out_ops.h"


#if (defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__))
    #define _is_windows
#elif defined(__linux__)
    #define _is_linux
#endif
#ifdef _is_windows
//Silly little Microsoft Windows magic...
extern "C" { // Tells the compiler to treat this as C code instead of C++ To avoid function overload
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport)
        int __stdcall ReadConsoleA(
            void*,
            const void*,
            unsigned long,
            unsigned long*,
            void* /*Why do we need this if it is always nullptr?*/); // Yeah, I do not know why we need all of these
    /*This appears to be declared in <windows.h>, yet no definition
     *is found because it is an internal part of Windows
     *
     */
}

//STD_INPUT_HANDLE as seen in <windows.h>
#define STD_INPUT_HANDLE ((unsigned long) - 10)
/* What???
 * stdin = -10
 * stdout = -11
 * stderr = -12
 */
#endif

static void rc_read(char* _buf, unsigned long _buf_size) {
    unsigned long _read = 0;
#ifdef _is_windows
    void* _input_handler = GetStdHandle(STD_INPUT_HANDLE);
    ReadConsoleA(_input_handler, _buf, _buf_size - 1, &_read, nullptr);
#elif defined(_is_linux)
#endif
    //Terminate manually because Windows does not do this for you
    _buf[_read] = '\0';
    //Strip possible newline character from 'Enter'
    if (_read > 0 && _buf[_read - 1] == '\n') _buf[_read - 1] = '\0';
}

/*
  Read the console...
  char* _buf - Response will be written to this
 */
void c_cio_read(char* _buf, unsigned long _buf_size) {
    return rc_read(_buf, _buf_size);
}

#undef STD_INPUT_HANDLE
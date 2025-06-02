//
// Created by David on 6/1/2025.
//

#include "../str_write.h"
#include "../../ops/cio_out_ops.h"

//Goofy Microsoft Windows magic...
extern "C" { // Tells the compiler to treat this as C code instead of C++ To avoid function overload
    __declspec(dllimport) void* __stdcall GetStdHandle(unsigned long);
    __declspec(dllimport)
        int __stdcall WriteConsoleA(
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

#define STD_OUTPUT_HANDLE ((unsigned int) - 11)
/* What???
 * stdin = -10
 * stdout = -11
 * stderr = -12
 */

//Internal function for writing to the console - Use cio_write or io::cio_out << instead
static void rc_write(const char* _msg) {
    void* _output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned long _str_len = 0, _written;
    while (_msg[_str_len] != '\0')
        _str_len++;
    WriteConsoleA(_output_handler, _msg, _str_len, &_written, nullptr);
}

void rc_write(const float* _msg) {
    void* _output_handler = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned long _str_len = 0, _written;
    while (_msg[_str_len] != '\0')
        _str_len++;
    WriteConsoleA(_output_handler, _msg, _str_len, &_written, nullptr);
}

/*
  Write to the console...
  const char* _msg - Message to be written to the console - can be chained with string operator<<(string)
 */
void hexforge_cio_out::cio_write(const char* _msg) {
    return rc_write(_msg);
}

#undef STD_OUTPUT_HANDLE
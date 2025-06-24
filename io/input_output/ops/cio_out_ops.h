//
// Created by jhbg8 on 6/1/2025.
//

#ifndef CIO_OUT_OPS_H
//Include guard
#define CIO_OUT_OPS_H

#include "../../../string/string.h"

extern void c_cio_write(const char*);
extern void c_cio_read(char*, unsigned long);

class cio_out {
public:
    cio_out& operator<< (const hexforge_string::string&);
    cio_out& operator<<(const char*);
    cio_out& operator<<(char);
    cio_out& operator<<(int);
};

class cio_in {
public:
    cio_in& operator>> (const hexforge_string::string&);
};

#endif //CIO_OUT_OPS_H

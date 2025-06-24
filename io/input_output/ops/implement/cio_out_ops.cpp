//
// Created by jhbg8 on 6/1/2025.
//

#include "../cio_out_ops.h"

#include "../../../../string/to_string.h"

using namespace hexforge_string;

//Print a custom string _s via 'cio_out << _s'
cio_out& cio_out::operator<<(const string &_s) {
    c_cio_write(_s.data());
    return *this;
}
//Print a const char*-type string _str via 'cio_out << _str'
cio_out& cio_out::operator<<(const char* _str) {
    c_cio_write(_str);
    return *this;
}
//Print a single character _c via 'cio_out << _c'
cio_out& cio_out::operator<<(const char _c) {
    const char _buffer[2]{_c, '\0'}; /*If we do not do this,
                                     cio_write will not know
                                     to stop after the character,
                                     and will return garbage until
                                     it hits a '\0'*/
    c_cio_write(_buffer);
    return *this;
}
//Print a 32-bit signed integer _i via 'cio_out << _i'
cio_out& cio_out::operator<<(const int _i) {
    c_cio_write(hexforge_to_string::to_string(_i).data());
    return *this;
}
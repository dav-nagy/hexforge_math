//
// Created by jhbg8 on 6/1/2025.
//

#include "../cio_out_ops.h"

#include "../../../../string/to_string.h"

using namespace hexforge_cio_out;
using namespace hexforge_string;

cio_out& cio_out::operator<<(const string &_s) {
    cio_write(_s.data());
    return *this;
}
cio_out& cio_out::operator<<(const char* _str) {
    cio_write(_str);
    return *this;
}
cio_out& cio_out::operator<<(const char _c) {
    const char _buffer[2]{_c, '\0'}; /*If we do not do this,
                                     cio_write will not know
                                     to stop after the character,
                                     and will return garbage until
                                     it hits a '\0'*/
    cio_write(_buffer);
    return *this;
}

cio_out& cio_out::operator<<(const int _i) {
    cio_write(hexforge_to_string::to_string(_i).data());
    return *this;
}
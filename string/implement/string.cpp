//
// Created by David on 6/1/2025.
//

#include "../string.h"

using namespace hexforge_string;

[[nodiscard]] const char* string::data() const {return _data;};
[[nodiscard]] unsigned long string::length() const {return _length;};

void string::data(const char* _str) {
    unsigned long _l = 0; // Length
    while (_str[_l] != '\0')
        ++_l;
    _length = _l;
    _data = new char[_length + 1];
    for (unsigned long i = 0; i < _length; ++i)
        _data[i] = _str[i];
    _data[_length] = '\0';
}

string::string() {
    _data = new char[1]{'\0'};
    _length = 1;
};

string::string(const char* _str) {
    unsigned long _l = 0; // Length
    while (_str[_l] != '\0') ++_l;
    _length = _l;
    _data = new char[_length + 1];
    for (unsigned long _i = 0; _i < _length; ++_i)
        _data[_i] = _str[_i];
    _data[_length] = '\0';
}

string::string(const string& _str) {
    _length = _str._length;
    _data = new char[_length + 1];
    for (unsigned long _i = 0; _i < _length; ++_i)
        _data[_i] = _str._data[_i];
    _data[_length] = '\0';
}

string::string(const char _c) {
    _length = 1;
    _data = new char[2]{_c, '\0'};
}

string::~string() { delete[] _data; }

string& string::operator=(const string& _str) {
    if (this == &_str)
        return *this;
    delete[] _data;
    _length = _str._length;
    for (unsigned long _i = 0; _i < _length; ++_i)
        _data[_i] = _str[_i];
    _data[_length] = '\0';
    return *this;
}
char string::operator[] (const unsigned long _i) const {
    return _data[_i];
}

string& string::operator+=(const string& _s) {
    char* _r = new char[_length + _s._length + 1];
    for (unsigned long _i = 0; _i < _length; ++_i)
        _r[_i] = _data[_i];
    for (unsigned long _i = 0; _i < _s._length + 1; ++_i)
        _r[_length + _i] = _s[_i];
    _r[_length + _s._length] = '\0';
    delete[] _data;
    _data = _r;
    _length += _s._length;
    return *this;
}

string& string::operator<<(const string& _s){
    *this += _s;
    return *this;
}
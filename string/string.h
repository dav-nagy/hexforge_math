//
// Created by David on 6/1/2025.
//

#ifndef STRING_H
//Include guard
#define STRING_H

namespace hexforge_string {
    class string {
    private:
        char* _data;
        unsigned long _length;
    public:

        [[nodiscard]] const char* data() const;
        [[nodiscard]] unsigned long length() const;

        void data(const char* _str);

        string();
        explicit string(const char*);
        string(const string&);

        explicit string(const char);

        ~string();

        string& operator=(const string&);
        char operator[](unsigned long) const;

        string& operator+=(const string&);

        string& operator<<(const string&);
    };
}
#endif //STRING_H

#include <iostream>
#include "system.h"
// The string implementation for type 'String'
types::String types::String::replace(types::String &from, types::String &to) {
    size_t start_pos = std::string::find(from);
    if(start_pos == std::string::npos)
        return *this;
    std::string::replace(start_pos, from.length(), to);
    return *this;
}

types::String types::String::replace_all(types::String &from, types::String &to) {
    if(from.empty())
        return *this;
    size_t start_pos = 0;
    while((start_pos = std::string::find(from, start_pos)) != std::string::npos) {
        std::string::replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
    return *this;
}

#pragma once

#include <string>

namespace types {
    class String : public std::string {
    public:
        String(const char* c) : std::string(c) {}
        String replace (String& from, String& to);
        String replace_all (String& from, String& to);
    };
}
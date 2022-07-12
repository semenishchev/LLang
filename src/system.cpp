#include <iostream>

// The string implementation for type 'String'
class String : public std::string {
    public:
        String(const char* c) : std::string(c) {}
        String replace (String from, String to) {
            size_t start_pos = std::string::find(from);
            if(start_pos == std::string::npos)
                return *this;
            std::string::replace(start_pos, from.length(), to);
            return *this;
        }

        String replaceAll (String from, String to) {
            if(from.empty())
                return *this;
            size_t start_pos = 0;
            while((start_pos = std::string::find(from, start_pos)) != std::string::npos) {
                std::string::replace(start_pos, from.length(), to);
                start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
            return *this;   
        }
};
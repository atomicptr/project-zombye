#include <cxxabi.h>
#include <iostream>
#include <stdexcept>

#include <zombye/utils/demangle.hpp>

namespace zombye {
    std::string demangle(const char* name) {
        char* realname;
        std::size_t len;
        int stat;
        realname = abi::__cxa_demangle(name, nullptr, &len, &stat);
        if (realname) {
            std::string out(realname);
            std::free(realname);
            if (out.find("zombye") != std::string::npos) {
                return out.substr(8, out.length());
            }
            return out;
        }
        return std::string(name);
    }
}

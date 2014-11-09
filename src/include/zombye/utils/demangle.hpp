#ifndef __ZOMBYE_DEMANGLE_HPP__
#define __ZOMBYE_DEMANGLE_HPP__

#include <string>

#include <cxxabi.h>

namespace zombye {
    std::string demangle(const char* name);
}

#endif

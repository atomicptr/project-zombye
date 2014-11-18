#ifndef __ZOMBYE_INPUT_MANAGER_HPP__
#define __ZOMBYE_INPUT_MANAGER_HPP__

#include <cmath>

#include <zombye/input/input_system.hpp>

namespace zombye {
    class input_system;

    class input_manager {
    public:
        input_manager(input_system*);

    private:
        input_system *input_;
    };
}

#endif

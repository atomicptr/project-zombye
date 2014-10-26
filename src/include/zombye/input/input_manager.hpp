#ifndef __ZOMBYE_INPUT_MANAGER_HPP__
#define __ZOMBYE_INPUT_MANAGER_HPP__

#include <cmath>

#include <zombye/input/input_system.hpp>

namespace zombye {
    class input_system;

    class input_manager {
    public:
        input_manager(input_system*);

        float axis_x() const;
        float axis_y() const;

        // game specific input
        bool fire() const;
        bool reload() const;

        // menu specific input
        bool up() const;
        bool down() const;
        bool left() const;
        bool right() const;

        bool confirm() const;
        bool cancel() const;

    private:
        input_system *input_;
    };
}

#endif
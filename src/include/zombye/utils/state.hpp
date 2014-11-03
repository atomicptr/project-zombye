#ifndef __ZOMBYE_STATE_HPP__
#define __ZOMBYE_STATE_HPP__

#include <zombye/utils/state_machine.hpp>

namespace zombye {
    class state_machine;

    class state {
    public:
        virtual void enter() = 0;
        virtual void leave() = 0;
        virtual void update(float) {}
    };
}

#endif
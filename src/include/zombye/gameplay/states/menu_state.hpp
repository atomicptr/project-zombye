#ifndef __ZOMBYE_MENU_STATE_HPP__
#define __ZOMBYE_MENU_STATE_HPP__

#include <zombye/utils/state.hpp>
#include <zombye/utils/state_machine.hpp>

#include <zombye/utils/logger.hpp>

namespace zombye {
    class menu_state : public zombye::state {
    public:
        menu_state(zombye::state_machine*);

        void enter();
        void leave();
        void update(float);

    private:
        zombye::state_machine *sm_;
    };
}

#endif
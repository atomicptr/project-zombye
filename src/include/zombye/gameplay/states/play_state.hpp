#ifndef __ZOMBYE_PLAY_STATE_HPP__
#define __ZOMBYE_PLAY_STATE_HPP__

#include <zombye/utils/state.hpp>
#include <zombye/utils/state_machine.hpp>
#include <zombye/input/input_manager.hpp>

#include <zombye/gameplay/command.hpp>

#include <zombye/utils/logger.hpp>

namespace zombye {
    class play_state : public zombye::state {
    public:
        play_state(zombye::state_machine*);

        void enter();
        void leave();
        void update(float);

    private:
        state_machine *sm_;
        input_manager *input_;
    };
}

#endif

#ifndef __ZOMBYE_GAMEPLAY_SYSTEM_HPP__
#define __ZOMBYE_GAMEPLAY_SYSTEM_HPP__

#include <string>

#include <zombye/gameplay/game_states.hpp>
#include <zombye/gameplay/states/menu_state.hpp>

#include <zombye/core/game.hpp>
#include <zombye/utils/state_machine.hpp>

namespace zombye {
    class game;
    class state_machine;

    class gameplay_system {
    public:
        gameplay_system(game*);

        void use(std::string);
        void dispose_current();

        void update(float);
    private:
        std::unique_ptr<zombye::state_machine> sm_;

        void init_game_states();
    };
}

#endif
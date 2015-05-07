#include <zombye/core/game.hpp>
#include <zombye/gameplay/game_states.hpp>
#include <zombye/gameplay/gameplay_system.hpp>
#include <zombye/gameplay/states/menu_state.hpp>
#include <zombye/gameplay/states/play_state.hpp>
#include <zombye/utils/state_machine.hpp>

zombye::gameplay_system::gameplay_system(zombye::game *game) {
    sm_ = std::unique_ptr<zombye::state_machine>(new zombye::state_machine(game));

    init_game_states();
}

void zombye::gameplay_system::init_game_states() {
    sm_->add<zombye::menu_state>(GAME_STATE_MENU);
    sm_->add<zombye::play_state>(GAME_STATE_PLAY);
}

void zombye::gameplay_system::use(std::string name) {
    sm_->use(name);
}

void zombye::gameplay_system::dispose_current() {
    sm_->dispose_current();
}

void zombye::gameplay_system::update(float delta_time) {
    sm_->update(delta_time);
}

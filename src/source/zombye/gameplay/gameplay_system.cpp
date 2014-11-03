#include <zombye/gameplay/gameplay_system.hpp>

zombye::gameplay_system::gameplay_system(zombye::game *game) {
    sm_ = std::unique_ptr<zombye::state_machine>(new zombye::state_machine(game));

    init_game_states();
}

void zombye::gameplay_system::init_game_states() {
    sm_->add<zombye::menu_state>(GAME_STATE_MENU);
}

void zombye::gameplay_system::use(std::string name) {
    sm_->use(name);
}

void zombye::gameplay_system::update(float delta_time) {
    sm_->update(delta_time);
}
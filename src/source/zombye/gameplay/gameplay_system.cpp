#include <zombye/gameplay/gameplay_system.hpp>

zombye::gameplay_system::gameplay_system(zombye::game *game) {
    sm = std::unique_ptr<zombye::state_machine>(new zombye::state_machine(game));

    init_game_states();
}

void zombye::gameplay_system::init_game_states() {
    //sm->add();
}

void zombye::gameplay_system::use(std::string name) {
    sm->use(name);
}

void zombye::gameplay_system::update(float delta_time) {
    sm->update(delta_time);
}
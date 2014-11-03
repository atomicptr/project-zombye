#include <zombye/gameplay/states/play_state.hpp>

zombye::play_state::play_state(zombye::state_machine *sm) : sm_(sm) {
}

void zombye::play_state::enter() {
    zombye::log("enter play state");
}

void zombye::play_state::leave() {
    zombye::log("leave play state");
}

void zombye::play_state::update(float delta_time) {
}
#include <zombye/gameplay/states/menu_state.hpp>

zombye::menu_state::menu_state(zombye::state_machine *sm) : sm_(sm) {

}

void zombye::menu_state::enter() {
    zombye::log("enter menu state");
}

void zombye::menu_state::leave() {
    zombye::log("leave menu state");
}

void zombye::menu_state::update(float delta_time) {

}
#include <zombye/gameplay/states/play_state.hpp>

class test_command : public zombye::command {
public:
    test_command() {}

    void execute() {
        zombye::log("FIRE!!!! PENG PENG!!!");
    }
};

zombye::play_state::play_state(zombye::state_machine *sm) : sm_(sm) {
    auto input = sm->get_game()->input();
    input_ = input->create_manager();

    input_->register_command("FIRE", new test_command());

    input_->register_event("FIRE", input->first_joystick()->button_A());
    input_->register_event("FIRE", input->first_joystick()->button_B());
}

void zombye::play_state::enter() {
    zombye::log("enter play state");
}

void zombye::play_state::leave() {
    zombye::log("leave play state");
}

void zombye::play_state::update(float delta_time) {
}

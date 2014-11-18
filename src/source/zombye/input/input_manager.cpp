#include <zombye/input/input_manager.hpp>

zombye::input_manager::input_manager(input_system *input) : input_(input) {
}

void zombye::input_manager::register_event(std::string event_name, zombye::button &btn) {
    btn.register_keydown_listener([event_name, that=this](button &b) {
        that->commands_.at(event_name)->execute();
    });
}

void zombye::input_manager::register_keyboard_event(std::string event_name, std::string key) {
    input_->keyboard()->register_keydown_listener(key, [event_name, that=this]() {
        that->commands_.at(event_name)->execute();
    });
}

void zombye::input_manager::register_command(std::string event_name, zombye::command *cmd) {
    commands_.insert(std::make_pair(event_name, cmd));
}

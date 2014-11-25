#include <zombye/input/button.hpp>

zombye::button::button() : was_pressed(false), was_just_pressed(false) {
}

bool zombye::button::pressed() const {
    return was_pressed;
}

bool zombye::button::just_pressed() {
    if(!was_just_pressed && was_pressed) {
        was_just_pressed = true;
        return true;
    }

    return false;
}

void zombye::button::key_down() {
    was_pressed = true;

    for(auto &listener : listeners_) {
        listener(*this);
    }
}

void zombye::button::key_up() {
    was_pressed = false;
    was_just_pressed = false;
}

void zombye::button::register_keydown_listener(std::function<void(zombye::button&)> listener) {
    listeners_.push_back(listener);
}

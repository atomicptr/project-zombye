#include <zombye/input/keyboard.hpp>

zombye::keyboard::keyboard() {
}

bool zombye::keyboard::pressed(std::string key_str) {
    auto code = SDL_GetScancodeFromName(key_str.c_str());

    return was_pressed_[code];
}

bool zombye::keyboard::just_pressed(std::string key_str) {
    auto code = SDL_GetScancodeFromName(key_str.c_str());

    if(!was_just_pressed_[code] && was_pressed_[code]) {
        was_just_pressed_[code] = true;
        return true;
    }

    return false;
}

void zombye::keyboard::update(SDL_Event &event) {
    auto code = event.key.keysym.scancode;

    auto it = was_pressed_.find(code);

    if(it == was_pressed_.end()) {
        was_pressed_[code] = false;
        was_just_pressed_[code] = false;
    }

    if(event.key.state == SDL_PRESSED) {
        was_pressed_[code] = true;
    } else if(event.key.state == SDL_RELEASED) {
        was_pressed_[code] = false;
        was_just_pressed_[code] = false;
    }
}
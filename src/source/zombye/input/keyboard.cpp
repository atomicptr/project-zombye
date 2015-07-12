#include <zombye/input/keyboard.hpp>

zombye::keyboard::keyboard() {
}

bool zombye::keyboard::pressed(std::string key_str) {
    auto code = SDL_GetScancodeFromName(key_str.c_str());

    return state_[code];
}

bool zombye::keyboard::just_pressed(std::string key_str) {
    auto code = SDL_GetScancodeFromName(key_str.c_str());

    if(!was_just_pressed_[code] && state_[code]) {
        was_just_pressed_[code] = true;
        return true;
    }

    return false;
}

void zombye::keyboard::register_keydown_listener(std::string key, std::function<void()> listener, bool continuous) {
    auto code = SDL_GetScancodeFromName(key.c_str());

    auto it = keydown_listeners_.find(code);

    if(it == keydown_listeners_.end()) {
        (continuous ? cont_keydown_listeners_[code] : keydown_listeners_[code]) = std::vector<std::function<void()>>{};
    }

    auto &listeners = (continuous ? cont_keydown_listeners_[code] : keydown_listeners_[code]);

    listeners.push_back(listener);

    if(continuous) {
        observed_continuous_keys_.push_back(code);
    }
}

void zombye::keyboard::register_keyup_listener(std::string key, std::function<void()> listener, bool continuous) {
    auto code = SDL_GetScancodeFromName(key.c_str());

    auto it = keyup_listeners_.find(code);

    if(it == keyup_listeners_.end()) {
        (continuous ? cont_keyup_listeners_[code] : keyup_listeners_[code]) = std::vector<std::function<void()>>{};
    }

    auto &listeners = (continuous ? cont_keyup_listeners_[code] : keyup_listeners_[code]);

    listeners.push_back(listener);

    if(continuous) {
        observed_continuous_keys_.push_back(code);
    }
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

        notify_listeners(keydown_listeners_, code);
    } else if(event.key.state == SDL_RELEASED) {
        was_pressed_[code] = false;
        was_just_pressed_[code] = false;

        notify_listeners(keyup_listeners_, code);
    }
}

void zombye::keyboard::update_continuous() {
    state_ = SDL_GetKeyboardState(nullptr);

    for(auto &code : observed_continuous_keys_) {
        if(state_[code]) {
            was_continuous_pressed_[code] = true;
            notify_listeners(cont_keydown_listeners_, code);
        } else if(!state_[code] && was_continuous_pressed_[code]) {
            was_continuous_pressed_[code] = false;
            notify_listeners(cont_keyup_listeners_, code);
        }
    }
}

void zombye::keyboard::notify_listeners(sdl_keyevent_map &listeners, SDL_Scancode code) {
    auto v = listeners[code];

    for(auto &listener : v) {
        listener();
    }
}

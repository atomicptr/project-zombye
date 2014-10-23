#include <zombye/input/joystick.hpp>

zombye::joystick::joystick(int id, SDL_Joystick *joystick) : id_(id), joystick_(joystick) {
    // TODO: add logger
    std::cout << "opened joystick #" << id << " " << SDL_JoystickNameForIndex(id) << std::endl;
}

zombye::joystick::~joystick() {
    if(SDL_JoystickGetAttached(joystick_)) {
        SDL_JoystickClose(joystick_);
    }
}

void zombye::joystick::update(SDL_Event &event) {
    handle_xbox360(event);
}

// TODO: add some fancy input system
void zombye::joystick::handle_xbox360(SDL_Event &event) {
    auto value = 0.f;

    if(event.jaxis.value < 0) {
        value = event.jaxis.value / 32768.f;
    } else {
        value = event.jaxis.value / 32767.f;
    }

    // left stick right/left
    if(event.jaxis.axis == 0) {
        std::cout << "#1 lr: " << value << std::endl;
    }

    // left stick up/down
    if(event.jaxis.axis == 1) {
        std::cout << "#1 ud: " << value << std::endl;
    }

    // right stick right/left
    if(event.jaxis.axis == 2) {
        std::cout << "#2 lr: " << value << std::endl;
    }

    // right stick up/down
    if(event.jaxis.axis == 4) {
        std::cout << "#2 ud: " << value << std::endl;
    }
}
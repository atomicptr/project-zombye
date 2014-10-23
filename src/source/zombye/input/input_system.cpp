#include <zombye/input/input_system.hpp>

zombye::input_system::input_system() {
    if(!SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    // TODO: add logger
    std::cout << "init input system" << std::endl;

    detect_joysticks();
}

void zombye::input_system::update(SDL_Event &event) {
    switch(event.type) {
        case SDL_JOYAXISMOTION:
        case SDL_JOYHATMOTION:
        case SDL_JOYBALLMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            auto which = event.jaxis.which;

            joysticks_.at(which)->update(event);
    }
}

void zombye::input_system::detect_joysticks() {
    // TODO: add logger
    std::cout << "trying to detect joysticks..." << std::endl;

    const auto joystick_count = SDL_NumJoysticks();

    joysticks_.clear();

    if(joystick_count > 0) {

        for(auto i = 0; i < joystick_count; i++) {
            auto joystick_ptr = SDL_JoystickOpen(i);
            auto joystick = std::make_shared<zombye::joystick>(SDL_JoystickInstanceID(joystick_ptr), joystick_ptr);

            if(joystick_ptr) {
                joysticks_.insert(std::make_pair(i, joystick));
            } else {
                // TODO: add logger
                std::cerr << "Err: [Joystick] " << SDL_GetError() << std::endl;
            }
        }

        SDL_JoystickEventState(SDL_ENABLE);
    }

    // TODO: add logger
    std::cout << "Found " << joysticks_.size() << " joysticks." << std::endl;
}
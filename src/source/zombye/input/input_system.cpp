#include <zombye/input/input_system.hpp>

zombye::input_system::input_system(zombye::config_system *config) : config_system_(config) {
    if(!SDL_WasInit(SDL_INIT_JOYSTICK)) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    zombye::log("init input system");

    mouse_ = std::unique_ptr<zombye::mouse>(new zombye::mouse());
    keyboard_ = std::unique_ptr<zombye::keyboard>(new zombye::keyboard());

    detect_joysticks();
}

zombye::keyboard* zombye::input_system::keyboard() {
    return keyboard_.get();
}

zombye::mouse* zombye::input_system::mouse() {
    return mouse_.get();
}

zombye::joystick* zombye::input_system::joystick(int id) {
    return joysticks_.at(id).get();
}

zombye::joystick* zombye::input_system::first_joystick() {
    if(joysticks_.size() > 0) {
        return joysticks_.begin()->second.get();
    }

    return nullptr;
}

zombye::input_manager* zombye::input_system::create_manager() {
    managers_.emplace_back(new input_manager(this));

    return managers_.back().get();
}

std::vector<int> zombye::input_system::joystick_ids() const {
    static std::vector<int> id;
    static bool initial = true;

    if(initial) {
        for(auto &j : joysticks_) {
            id.push_back(j.first);
        }

        initial = false;
    }

    return id;
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

    switch(event.type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
            mouse_->update(event);
    }

    switch(event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyboard_->update(event);
    }
}

void zombye::input_system::detect_joysticks() {
    zombye::log("trying to detect joysticks...");

    const auto joystick_count = SDL_NumJoysticks();

    joysticks_.clear();

    if(joystick_count > 0) {

        for(auto i = 0; i < joystick_count; i++) {
            auto joystick_ptr = SDL_JoystickOpen(i);
            auto joystick = std::make_shared<zombye::joystick>(SDL_JoystickInstanceID(joystick_ptr), joystick_ptr, config_system_);

            if(joystick_ptr) {
                joysticks_.insert(std::make_pair(i, joystick));
            } else {
                zombye::log(LOG_ERROR, "[Joystick] " + std::string(SDL_GetError()));
            }
        }

        SDL_JoystickEventState(SDL_ENABLE);
    }

    zombye::log("Found " + std::to_string(joysticks_.size()) + " joysticks.");
}

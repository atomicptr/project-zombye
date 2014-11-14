#ifndef __ZOMBYE_INPUT_SYSTEM_HPP__
#define __ZOMBYE_INPUT_SYSTEM_HPP__

#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include <zombye/input/keyboard.hpp>
#include <zombye/input/joystick.hpp>
#include <zombye/input/mouse.hpp>
#include <zombye/input/input_manager.hpp>
#include <zombye/utils/logger.hpp>

#include <zombye/config/config_system.hpp>

namespace zombye {
    class input_manager;

    class input_system {
    public:
        input_system(zombye::config_system*);

        zombye::mouse* mouse();
        zombye::keyboard* keyboard();
        zombye::joystick* joystick(int);
        std::vector<int> joystick_ids() const;

        zombye::joystick* first_joystick();

        input_manager* manager() const;

        void update(SDL_Event&);
    private:
        std::map<int, std::shared_ptr<zombye::joystick>> joysticks_;
        std::unique_ptr<zombye::mouse> mouse_;
        std::unique_ptr<zombye::keyboard> keyboard_;
        std::unique_ptr<zombye::input_manager> manager_;
        config_system *config_system_;

        void detect_joysticks();
    };
}

#endif

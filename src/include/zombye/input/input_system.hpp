#ifndef __ZOMBYE_INPUT_SYSTEM_HPP__
#define __ZOMBYE_INPUT_SYSTEM_HPP__

#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include <zombye/input/joystick.hpp>
#include <zombye/input/mouse.hpp>

namespace zombye {

    class input_system {
    public:
        input_system();

        mouse* get_mouse();
        joystick* get_joystick(int);
        std::vector<int> ids() const;

        void update(SDL_Event&);
    private:
        std::map<int, std::shared_ptr<joystick>> joysticks_;
        std::unique_ptr<mouse> mouse_;

        void detect_joysticks();
    };
}

#endif
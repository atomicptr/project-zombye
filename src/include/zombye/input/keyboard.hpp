#ifndef __ZOMBYE_KEYBOARD_HPP__
#define __ZOMBYE_KEYBOARD_HPP__

#include <string>
#include <map>
#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

namespace zombye {
    class keyboard {
    public:
        keyboard();

        bool pressed(std::string);
        bool just_pressed(std::string);

        void update(SDL_Event&);

    private:
        std::map<SDL_Scancode, bool> was_pressed_;
        std::map<SDL_Scancode, bool> was_just_pressed_;
    };
}

#endif
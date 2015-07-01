#ifndef __ZOMBYE_KEYBOARD_HPP__
#define __ZOMBYE_KEYBOARD_HPP__

#include <string>
#include <map>
#include <iostream>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

namespace zombye {
    using sdl_keyevent_map = std::map<SDL_Scancode, std::vector<std::function<void()>>>;

    class keyboard {
    public:
        keyboard();

        bool pressed(std::string);
        bool just_pressed(std::string);

        void register_keydown_listener(std::string, std::function<void()>);
        void register_keyup_listener(std::string, std::function<void()>);

        void update(SDL_Event&);

    private:
        std::map<SDL_Scancode, bool> was_pressed_;
        std::map<SDL_Scancode, bool> was_just_pressed_;

        sdl_keyevent_map keydown_listeners_;
        sdl_keyevent_map keyup_listeners_;

        void notify_listeners(sdl_keyevent_map&, SDL_Scancode);
    };
}

#endif

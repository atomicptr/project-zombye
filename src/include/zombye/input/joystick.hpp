#ifndef __ZOMBYE_JOYSTICK_HPP__
#define __ZOMBYE_JOYSTICK_HPP__

#include <iostream>

#include <SDL2/SDL.h>

#include <zombye/utils/os.h>

namespace zombye {
    class joystick {
    public:
        joystick(int, SDL_Joystick*);
        ~joystick();

        void update(SDL_Event&);
    private:
        const int id_;
        SDL_Joystick *joystick_;

        void handle_xbox360(SDL_Event&);
    };
}

#endif
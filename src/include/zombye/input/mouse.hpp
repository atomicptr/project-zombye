#ifndef __ZOMBYE_MOUSE_HPP__
#define __ZOMBYE_MOUSE_HPP__

#include <SDL2/SDL.h>

#include <zombye/input/button.hpp>

namespace zombye {
    class mouse {
    public:
        mouse();

        void update(SDL_Event&);

        float x() const;
        float y() const;

        button& left_button();
        button& middle_button();
        button& right_button();
    private:
        float x_;
        float y_;

        button left_button_;
        button middle_button_;
        button right_button_;
    };
}

#endif
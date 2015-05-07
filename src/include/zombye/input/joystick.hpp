#ifndef __ZOMBYE_JOYSTICK_HPP__
#define __ZOMBYE_JOYSTICK_HPP__

#include <string>

#include <SDL2/SDL.h>

#include <zombye/input/button.hpp>
#include <zombye/utils/os.h>

namespace zombye {
    class config_system;
    class logger;
}

namespace zombye {
    struct stick {
        float x;
        float y;
    };

    class joystick {
    public:
        joystick(int, SDL_Joystick*, config_system*);
        ~joystick();

        void enable_profile(int, int, int, int, int, int, int, int, int, int, int,
            int, int, int, int, int, int, int, int, int, bool);

        void reset();
        void update(SDL_Event&);

        stick& left_stick();
        stick& right_stick();
        float left_trigger();
        float right_trigger();
        button& button_A();
        button& button_B();
        button& button_X();
        button& button_Y();
        button& button_LB();
        button& button_RB();
        button& button_LS();
        button& button_RS();
        button& button_START();
        button& button_SELECT();
        button& dpad_up();
        button& dpad_down();
        button& dpad_left();
        button& dpad_right();
    private:
        const int id_;
        SDL_Joystick *joystick_;

        stick left_stick_;
        stick right_stick_;
        float rtrigger_val_;
        float ltrigger_val_;
        button button_A_;
        button button_B_;
        button button_X_;
        button button_Y_;
        button button_LB_;
        button button_RB_;
        button button_LS_;
        button button_RS_;
        button button_START_;
        button button_SELECT_;
        button button_dpad_up_;
        button button_dpad_down_;
        button button_dpad_left_;
        button button_dpad_right_;

        int leftx_;
        int lefty_;
        int rightx_;
        int righty_;
        int rtrigger_;
        int ltrigger_;
        int a_;
        int b_;
        int x_;
        int y_;
        int lb_;
        int rb_;
        int ls_;
        int rs_;
        int start_;
        int select_;
        int dpad_up_;
        int dpad_down_;
        int dpad_left_;
        int dpad_right_;
        bool hats_are_buttons_;
    };
}

#endif

#ifndef __ZOMBYE_INPUT_SYSTEM_HPP__
#define __ZOMBYE_INPUT_SYSTEM_HPP__

#include <memory>
#include <map>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

namespace zombye {
    class config_system;
    class input_manager;
    class joystick;
    class keyboard;
    class logger;
    class mouse;
}

namespace zombye {
    class input_system {
    public:
        input_system(zombye::config_system*);

        zombye::mouse* mouse();
        zombye::keyboard* keyboard();
        zombye::joystick* joystick(int);
        std::vector<int> joystick_ids() const;

        zombye::joystick* first_joystick();

        input_manager* create_manager();

        void update(SDL_Event&);
        void update_continuous();
    private:
        std::map<int, std::shared_ptr<zombye::joystick>> joysticks_;
        std::unique_ptr<zombye::mouse> mouse_;
        std::unique_ptr<zombye::keyboard> keyboard_;
        config_system *config_system_;

        std::vector<std::unique_ptr<input_manager>> managers_;

        void detect_joysticks();
    };
}

#endif

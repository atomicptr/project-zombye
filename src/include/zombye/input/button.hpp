#ifndef __ZOMBYE_BUTTON_HPP__
#define __ZOMBYE_BUTTON_HPP__

#include <functional>
#include <vector>

namespace zombye {
    class button {
    public:
        button();

        bool pressed() const;
        bool just_pressed();
        void key_down();
        void key_up();

        void register_keydown_listener(std::function<void(button&)>);

    private:
        bool was_pressed;
        bool was_just_pressed;

        std::vector<std::function<void(button&)>> listeners_;
    };
}

#endif

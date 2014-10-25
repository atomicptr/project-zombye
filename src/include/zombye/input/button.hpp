#ifndef __ZOMBYE_BUTTON_HPP__
#define __ZOMBYE_BUTTON_HPP__

namespace zombye {
    class button {
    public:
        button();

        bool pressed() const;
        bool just_pressed();
        void key_down();
        void key_up();

    private:
        bool was_pressed;
        bool was_just_pressed;
    };
}

#endif
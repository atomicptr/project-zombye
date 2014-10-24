#include <zombye/input/mouse.hpp>

zombye::mouse::mouse() {
}

void zombye::mouse::update(SDL_Event &event) {
    if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(event.button.button == SDL_BUTTON_LEFT) {
            left_button_.key_down();
        } else if(event.button.button == SDL_BUTTON_MIDDLE) {
            middle_button_.key_down();
        } else if(event.button.button == SDL_BUTTON_RIGHT) {
            right_button_.key_down();
        }
    }

    if(event.type == SDL_MOUSEBUTTONUP) {
        if(event.button.button == SDL_BUTTON_LEFT) {
            left_button_.key_up();
        } else if(event.button.button == SDL_BUTTON_MIDDLE) {
            middle_button_.key_up();
        } else if(event.button.button == SDL_BUTTON_RIGHT) {
            right_button_.key_up();
        }
    }

    if(event.type == SDL_MOUSEMOTION) {
        x_ = event.motion.x;
        y_ = event.motion.y;
    }
}

float zombye::mouse::x() const {
    return x_;
}

float zombye::mouse::y() const {
    return y_;
}

zombye::button& zombye::mouse::left_button() {
    return left_button_;
}

zombye::button& zombye::mouse::middle_button() {
    return middle_button_;
}

zombye::button& zombye::mouse::right_button() {
    return right_button_;
}
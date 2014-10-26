#include <zombye/input/input_manager.hpp>

zombye::input_manager::input_manager(input_system *input) : input_(input) {
}

float zombye::input_manager::axis_x() const {
    auto joystick = input_->first_joystick();

    float val = 0.f;

    const float joystick_x = joystick ? joystick->left_stick().x : 0.f;

    const auto a_pressed = input_->keyboard()->pressed("a");
    const auto d_pressed = input_->keyboard()->pressed("d");

    if(a_pressed) {
        val = -1.f;
    }

    if(d_pressed) {
        val = 1.f;
    }

    if(joystick && !a_pressed && !d_pressed) {
        val = joystick_x;
    }

    return val;
}

float zombye::input_manager::axis_y() const {
    auto joystick = input_->first_joystick();

    float val = 0.f;

    const float joystick_y = joystick ? joystick->left_stick().y : 0.f;

    const auto w_pressed = input_->keyboard()->pressed("w");
    const auto s_pressed = input_->keyboard()->pressed("s");

    if(w_pressed) {
        val = -1.f;
    }

    if(s_pressed) {
        val = 1.f;
    }

    if(joystick && !w_pressed && !s_pressed) {
        val = joystick_y;
    }

    return val;
}

// game specific input
bool zombye::input_manager::fire() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->mouse()->left_button().pressed();

    if(joystick) {
        val = val || joystick->right_trigger() > 0.2f;
    }

    return val;
}

bool zombye::input_manager::reload() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("r");

    if(joystick) {
        val = val || joystick->button_X().just_pressed();
    }

    return val;
}

// menu specific input
// TODO: add just pressed behaviour to axis
bool zombye::input_manager::up() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("up");
    val = val || input_->keyboard()->just_pressed("w");

    if(joystick) {
        val = val || joystick->dpad_up().just_pressed();
        val = val || joystick->left_stick().y < -0.2f;
    }

    return val;
}

bool zombye::input_manager::down() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("down");
    val = val || input_->keyboard()->just_pressed("s");

    if(joystick) {
        val = val || joystick->dpad_down().just_pressed();
        val = val || joystick->left_stick().y > 0.2f;
    }

    return val;
}

bool zombye::input_manager::left() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("left");
    val = val || input_->keyboard()->just_pressed("a");

    if(joystick) {
        val = val || joystick->dpad_left().just_pressed();
        val = val || joystick->left_stick().x < -0.2f;
    }

    return val;
}

bool zombye::input_manager::right() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("right");
    val = val || input_->keyboard()->just_pressed("d");

    if(joystick) {
        val = val || joystick->dpad_right().just_pressed();
        val = val || joystick->left_stick().x > 0.2f;
    }

    return val;
}

bool zombye::input_manager::confirm() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("return");
    val = val || input_->keyboard()->just_pressed("space");

    if(joystick) {
        val = val || joystick->button_A().just_pressed();
    }

    return val;
}

bool zombye::input_manager::cancel() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->just_pressed("esc");
    val = val || input_->keyboard()->just_pressed("backspace");

    if(joystick) {
        val = val || joystick->button_B().just_pressed();
    }

    return val;
}
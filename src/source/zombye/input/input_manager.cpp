#include <zombye/input/input_manager.hpp>

zombye::input_manager::input_manager(input_system *input) : input_(input) {
}

float zombye::input_manager::axis_x() const {
    return .0f;
}

float zombye::input_manager::axis_y() const {
    return .0f;
}

// TODO: add "just_pressed"-functionality to directions
bool zombye::input_manager::up() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->pressed("up");
    val = val || input_->keyboard()->pressed("w");

    if(joystick) {
        val = val || joystick->dpad_up().pressed();
        val = val || joystick->left_stick().y < -0.2f;
    }

    return val;
}

bool zombye::input_manager::down() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->pressed("down");
    val = val || input_->keyboard()->pressed("s");

    if(joystick) {
        val = val || joystick->dpad_down().pressed();
        val = val || joystick->left_stick().y > 0.2f;
    }

    return val;
}

bool zombye::input_manager::left() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->pressed("left");
    val = val || input_->keyboard()->pressed("a");

    if(joystick) {
        val = val || joystick->dpad_left().pressed();
        val = val || joystick->left_stick().x < -0.2f;
    }

    return val;
}

bool zombye::input_manager::right() const {
    auto val = false;

    auto joystick = input_->first_joystick();

    val = val || input_->keyboard()->pressed("right");
    val = val || input_->keyboard()->pressed("d");

    if(joystick) {
        val = val || joystick->dpad_right().pressed();
        val = val || joystick->left_stick().x > 0.2f;
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
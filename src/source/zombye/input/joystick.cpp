#include <zombye/input/joystick.hpp>

zombye::joystick::joystick(int id, SDL_Joystick *joystick, zombye::config_system *config) : id_(id), joystick_(joystick) {
    zombye::log("opened joystick #" + std::to_string(id) + " " + std::string(SDL_JoystickNameForIndex(id)));

    if(config->has("gamepad")) {
        auto profile = config->get("gamepad", "config");
        auto hats_are_buttons = config->get("gamepad", "hats_are_buttons").asBool();

        auto p = [&](std::string name) -> int {
            return profile[name].asInt();
        };

        enable_profile(
            p("LS_X"), p("LS_Y"), p("RS_X"), p("RS_Y"), p("RT"), p("LT"), p("A"), p("B"),
            p("X"), p("Y"), p("LB"), p("RB"), p("LS"), p("RS"), p("START"), p("SELECT"),
            p("DPADUP"), p("DPADDOWN"), p("DPADLEFT"), p("DPADRIGHT"), hats_are_buttons
        );
    }
}

zombye::joystick::~joystick() {
    if(SDL_JoystickGetAttached(joystick_)) {
        SDL_JoystickClose(joystick_);
    }
}

void zombye::joystick::enable_profile(int leftx, int lefty, int rightx, int righty, int rtrigger,
        int ltrigger, int a, int b, int x, int y, int lb, int rb, int ls, int rs, int start,
        int select, int up, int down, int left, int right, bool hats_are_buttons) {
    leftx_ = leftx;
    lefty_ = lefty;
    rightx_ = rightx;
    righty_ = righty;
    rtrigger_ = rtrigger;
    ltrigger_ = ltrigger;
    a_ = a;
    b_ = b;
    x_ = x;
    y_ = y;
    lb_ = lb;
    rb_ = rb;
    ls_ = ls;
    rs_ = rs;
    start_ = start;
    select_ = select;
    dpad_up_ = up;
    dpad_down_ = down;
    dpad_left_ = left;
    dpad_right_ = right;
    hats_are_buttons_ = hats_are_buttons;
}

void zombye::joystick::reset() {
}

void zombye::joystick::update(SDL_Event &event) {
    auto value = 0.f;

    if(event.jaxis.value < 0) {
        value = event.jaxis.value / 32768.f;
    } else {
        value = event.jaxis.value / 32767.f;
    }

    // left stick right/left
    if(event.jaxis.axis == leftx_) {
        left_stick_.x = value;
    }

    // left stick up/down
    if(event.jaxis.axis == lefty_) {
        left_stick_.y = value;
    }

    // right stick right/left
    if(event.jaxis.axis == rightx_) {
        right_stick_.x = value;
    }

    // right stick up/down
    if(event.jaxis.axis == righty_) {
        right_stick_.y = value;
    }

    // right trigger
    if(event.jaxis.axis == rtrigger_) {
        value += 1.f;
        value /= 2.f;

        rtrigger_val_ = value;
    }

    // left trigger
    if(event.jaxis.axis == ltrigger_) {
        value += 1.f;
        value /= 2.f;

        ltrigger_val_ = value;
    }

    if(event.type == SDL_JOYBUTTONDOWN) {
        auto button = event.jbutton.button;

        if(button == a_) {
            button_A_.key_down();
        } else if(button == b_) {
            button_B_.key_down();
        } else if(button == x_) {
            button_X_.key_down();
        } else if(button == y_) {
            button_Y_.key_down();
        } else if(button == lb_) {
            button_LB_.key_down();
        } else if(button == rb_) {
            button_RB_.key_down();
        } else if(button == ls_) {
            button_LS_.key_down();
        } else if(button == rs_) {
            button_RS_.key_down();
        } else if(button == start_) {
            button_START_.key_down();
        } else if(button == select_) {
            button_SELECT_.key_down();
        } else if(hats_are_buttons_ && button == dpad_up_) {
            button_dpad_up_.key_down();
        } else if(hats_are_buttons_ && button == dpad_down_) {
            button_dpad_down_.key_down();
        } else if(hats_are_buttons_ && button == dpad_left_) {
            button_dpad_left_.key_down();
        } else if(hats_are_buttons_ && button == dpad_right_) {
            button_dpad_right_.key_down();
        }
    }

    if(event.type == SDL_JOYBUTTONUP) {
        auto button = event.jbutton.button;

        if(button == a_) {
            button_A_.key_up();
        } else if(button == b_) {
            button_B_.key_up();
        } else if(button == x_) {
            button_X_.key_up();
        } else if(button == y_) {
            button_Y_.key_up();
        } else if(button == lb_) {
            button_LB_.key_up();
        } else if(button == rb_) {
            button_RB_.key_up();
        } else if(button == ls_) {
            button_LS_.key_up();
        } else if(button == rs_) {
            button_RS_.key_up();
        } else if(button == start_) {
            button_START_.key_up();
        } else if(button == select_) {
            button_SELECT_.key_up();
        } else if(hats_are_buttons_ && button == dpad_up_) {
            button_dpad_up_.key_up();
        } else if(hats_are_buttons_ && button == dpad_down_) {
            button_dpad_down_.key_up();
        } else if(hats_are_buttons_ && button == dpad_left_) {
            button_dpad_left_.key_up();
        } else if(hats_are_buttons_ && button == dpad_right_) {
            button_dpad_right_.key_up();
        }
    }

    if(event.type == SDL_JOYHATMOTION && !hats_are_buttons_) {
        auto hat = event.jhat.hat;
        auto hvalue = int{event.jhat.value};

        static auto handle_button = [](button &b, int val) {
            if(val > 0) {
                b.key_down();
            } else {
                b.key_up();
            }
        };

        if(hat == dpad_up_) {
            handle_button(button_dpad_up_, hvalue);
        } else if(hat == dpad_down_) {
            handle_button(button_dpad_down_, hvalue);
        } else if(hat == dpad_left_) {
            handle_button(button_dpad_left_, hvalue);
        } else if(hat == dpad_right_) {
            handle_button(button_dpad_right_, hvalue);
        }
    }
}

zombye::stick& zombye::joystick::left_stick() {
    return left_stick_;
}

zombye::stick& zombye::joystick::right_stick() {
    return right_stick_;
}

float zombye::joystick::left_trigger() {
    return ltrigger_val_;
}

float zombye::joystick::right_trigger() {
    return rtrigger_val_;
}

zombye::button& zombye::joystick::button_A() {
    return button_A_;
}

zombye::button& zombye::joystick::button_B() {
    return button_B_;
}

zombye::button& zombye::joystick::button_X() {
    return button_X_;
}

zombye::button& zombye::joystick::button_Y() {
    return button_Y_;
}

zombye::button& zombye::joystick::button_LB() {
    return button_LB_;
}

zombye::button& zombye::joystick::button_RB() {
    return button_RB_;
}

zombye::button& zombye::joystick::button_LS() {
    return button_LS_;
}

zombye::button& zombye::joystick::button_RS() {
    return button_RS_;
}

zombye::button& zombye::joystick::button_START() {
    return button_START_;
}

zombye::button& zombye::joystick::button_SELECT() {
    return button_SELECT_;
}

zombye::button& zombye::joystick::dpad_up() {
    return button_dpad_up_;
}

zombye::button& zombye::joystick::dpad_down() {
    return button_dpad_down_;
}

zombye::button& zombye::joystick::dpad_left() {
    return button_dpad_left_;
}

zombye::button& zombye::joystick::dpad_right() {
    return button_dpad_right_;
}

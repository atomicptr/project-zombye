#include <zombye/input/stick.hpp>

zombye::stick::stick() {
    x_ = 0.f;
    y_ = 0.f;
}

void zombye::stick::update_x(float x) {
    x_ = x;
}

void zombye::stick::update_y(float y) {
    y_ = y;
}

float zombye::stick::x() const {
    return x_;
}

float zombye::stick::y() const {
    return y_;
}
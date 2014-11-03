#include <zombye/utils/state_machine.hpp>

zombye::state_machine::state_machine(zombye::game* game) : game_(game) {
}

zombye::state_machine::~state_machine() {
    if(current_ != nullptr) {
        current_->leave();
    }
}

void zombye::state_machine::remove(std::string name) {
    states_.erase(name);
}

void zombye::state_machine::use(std::string name) {
    if(has(name)) {
        if(current_ != nullptr) {
            current_->leave();
        }

        current_ = states_.at(name).get();
        current_->enter();
    } else {
        zombye::log(LOG_ERROR, "Unknown state: " + name);
    }
}

bool zombye::state_machine::has(std::string name) const {
    auto it = states_.find(name);

    return it != states_.end();
}

void zombye::state_machine::dispose_current() {
    if(current_ != nullptr) {
        current_->leave();
    }

    current_ = nullptr;
}

void zombye::state_machine::update(float delta_time) const {
    if(current_ != nullptr) {
        current_->update(delta_time);
    }
}

zombye::state* zombye::state_machine::current() const noexcept {
    return current_;
}

zombye::game* zombye::state_machine::game() const {
    return game_;
}
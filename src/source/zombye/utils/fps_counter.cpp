#include <zombye/utils/fps_counter.hpp>

zombye::fps_counter::fps_counter() : smooth_factor_(0.1f), delta_smoothed_(0.f), time_since_last_output_(0.f) {

}

void zombye::fps_counter::update(float delta_time) {
    delta_smoothed_ = (1.0f - smooth_factor_) * delta_smoothed_ + smooth_factor_ * delta_time;

    time_since_last_output_ += delta_time;

    if(time_since_last_output_ >= 1.0f) {
        time_since_last_output_ = 0.f;

        fps_ = int(((1.0f / delta_smoothed_) * 10.0f) / 10.0f);
        ms_per_frame_ = int((delta_smoothed_ * 10000.0f) / 10.0f);

        fire_listeners();
    }
}

int zombye::fps_counter::fps() const {
    return fps_;
}

int zombye::fps_counter::ms_per_frame() const {
    return ms_per_frame_;
}

void zombye::fps_counter::on_fps_output(std::function<void(fps_counter&)> listener) {
    listeners_.push_back(listener);
}

void zombye::fps_counter::fire_listeners() {
    for(auto& listener : listeners_) {
        listener(*this);
    }
}

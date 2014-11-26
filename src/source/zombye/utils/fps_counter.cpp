#include <zombye/utils/fps_counter.hpp>

zombye::fps_counter::fps_counter() : frames_per_second_(0.f), framecount_(0) {
    last_frame_time_ = SDL_GetTicks();
}

void zombye::fps_counter::update() {
    auto frame_index = framecount_ % FRAME_NUM;

    auto ticks = SDL_GetTicks();

    frame_times_[frame_index] = ticks - last_frame_time_;

    last_frame_time_ = SDL_GetTicks();

    framecount_++;
}

float zombye::fps_counter::fps() const {
    auto count = FRAME_NUM;

    if(framecount_ < FRAME_NUM) {
        count = framecount_;
    }

    auto fps = 0;

    for(size_t i = 0; i < count; i++) {
        fps += frame_times_[i];
    }

    fps /= count;

    return 1000.f / fps;
}

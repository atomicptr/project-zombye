#ifndef __ZOMBYE_FPS_COUNTER_HPP__
#define __ZOMBYE_FPS_COUNTER_HPP__

#include <SDL2/SDL.h>

#include <array>

#define FRAME_NUM 1000

namespace zombye {
    class fps_counter {
    public:
        fps_counter();

        void update();

        float fps() const;

    private:
        unsigned int framecount_;
        unsigned int last_frame_time_;

        std::array<unsigned int, FRAME_NUM> frame_times_;
    };
}

#endif

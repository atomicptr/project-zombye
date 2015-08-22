#ifndef __ZOMBYE_FPS_COUNTER_HPP__
#define __ZOMBYE_FPS_COUNTER_HPP__

#include <functional>
#include <vector>

namespace zombye {
    class fps_counter {
    public:
        fps_counter();
        void update(float);
        int fps() const;
        int ms_per_frame() const;

        void on_fps_output(std::function<void(fps_counter&)>);
    private:
        const float smooth_factor_;
        float delta_smoothed_;
        float time_since_last_output_;
        int fps_;
        int ms_per_frame_;

        std::vector<std::function<void(fps_counter&)>> listeners_;

        void fire_listeners();
    };
}

#endif

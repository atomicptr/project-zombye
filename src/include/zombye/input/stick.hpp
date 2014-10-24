#ifndef __ZOMBYE_STICK_HPP__
#define __ZOMBYE_STICK_HPP__

namespace zombye {
    class stick {
    public:
        stick();

        void update_x(float);
        void update_y(float);

        float x() const;
        float y() const;
    private:
        float x_;
        float y_;
    };
}

#endif
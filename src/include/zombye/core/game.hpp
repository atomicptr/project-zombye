#ifndef __ZOMBYE_GAME_HPP__
#define __ZOMBYE_GAME_HPP__

#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include <zombye/utils/sdlhelper.hpp>

namespace zombye {
    class game {
    public:
        game(std::string, int, int);
        ~game();

        void run();

        int width() const;
        int height() const;
    private:
        std::string title_;
        int width_;
        int height_;

        bool running_;
    };
}

#endif
#ifndef __ZOMBYE_MUSIC_HPP__
#define __ZOMBYE_MUSIC_HPP__

#include <string>
#include <memory>

#include <SDL2/SDL_mixer.h>

#include <zombye/assets/asset_manager.hpp>
#include <zombye/utils/audiohelper.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {

    class music {
    public:
        music(std::string);
        ~music();

        Mix_Music* content();
    private:
        std::shared_ptr<Mix_Music> music_;
    };
}

#endif
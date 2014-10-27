#ifndef __ZOMBYE_MUSIC_COLLECTION_HPP__
#define __ZOMBYE_MUSIC_COLLECTION_HPP__

#include <map>
#include <string>
#include <utility>

#include <SDL2/SDL_mixer.h>

#include <zombye/assets/asset_manager.hpp>
#include <zombye/audio/audiohelper.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {

    class music_collection {
    public:
        music_collection();

        Mix_Music* get(std::string);

        void add(std::string, std::string);
    private:
        std::map<std::string, std::shared_ptr<Mix_Music>> music_;
    };
}

#endif
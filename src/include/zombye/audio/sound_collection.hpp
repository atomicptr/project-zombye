#ifndef __ZOMBYE_SOUND_COLLECTION_HPP__
#define __ZOMBYE_SOUND_COLLECTION_HPP__

#include <map>
#include <string>
#include <utility>

#include <SDL2/SDL_mixer.h>

namespace zombye {

    class sound_collection {
    public:
        sound_collection();

        Mix_Chunk* get(std::string);

        void add(std::string, std::string);
    private:
        std::map<std::string, std::shared_ptr<Mix_Chunk>> sounds_;
    };
}

#endif

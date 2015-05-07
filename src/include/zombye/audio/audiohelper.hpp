#ifndef __ZOMBYE_AUDIOHELPER_HPP__
#define __ZOMBYE_AUDIOHELPER_HPP__

#include <SDL2/SDL_mixer.h>

namespace zombye {
    class asset;
    class logger;
}

namespace zombye {
    SDL_RWops* get_raw_from_asset(asset*);
}

#endif

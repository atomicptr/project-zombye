#ifndef __ZOMBEYE_SDLHELPER_HPP__
#define __ZOMBEYE_SDLHELPER_HPP__

#include <utility>

#include <SDL2/SDL.h>

#include <zombye/utils/make_smart.hpp>

namespace zombye {

    template<typename... Arguments>
    auto make_window(Arguments... args) {
        return make_smart<SDL_Window>(SDL_CreateWindow,
            SDL_DestroyWindow, std::forward<Arguments>(args)...);
    }
}

#endif

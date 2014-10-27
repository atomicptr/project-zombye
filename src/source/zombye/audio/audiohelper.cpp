#include <zombye/audio/audiohelper.hpp>

SDL_RWops* zombye::get_raw_from_asset(zombye::asset *asset) {
    auto &content = asset->content();

    auto raw = SDL_RWFromConstMem(content.data(), content.size());

    if(!raw) {
        zombye::log(LOG_ERROR, "Could not load audio from memory " + std::string(Mix_GetError()));
    }

    return raw;
}
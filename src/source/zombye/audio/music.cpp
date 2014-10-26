#include <zombye/audio/music.hpp>

zombye::music::music(std::string path) {
    zombye::asset_manager manager;

    auto asset = manager.load(path);

    auto raw = zombye::get_raw_from_asset(asset.get());

    music_ = std::shared_ptr<Mix_Music>(Mix_LoadMUS_RW(raw, 1), [](Mix_Music *ptr) {
        if(ptr != nullptr) {
            // TODO: possible leak
            //Mix_FreeMusic(ptr);
        }
    });

    if(!music_) {
        zombye::log(LOG_ERROR, "Could not read music " + std::string(Mix_GetError()));
    }
}

zombye::music::~music() {
}

Mix_Music* zombye::music::content() {
    return music_.get();
}
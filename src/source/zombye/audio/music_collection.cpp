#include <zombye/audio/music_collection.hpp>

zombye::music_collection::music_collection() {
}

Mix_Music* zombye::music_collection::get(std::string name) {
    if(music_.find(name) != music_.end()) {
        return music_[name].get();
    }

    return nullptr;
}

void zombye::music_collection::add(std::string name, std::string asset_path) {
    static zombye::asset_manager manager;

    auto asset = manager.load(asset_path);

    auto raw = zombye::get_raw_from_asset(asset.get());

    auto ptr = std::shared_ptr<Mix_Music>(Mix_LoadMUS_RW(raw, 1), Mix_FreeMusic);

    if(!ptr) {
        zombye::log(LOG_ERROR, "Could not read music " + std::string(Mix_GetError()));
    }

    music_.insert(std::make_pair(name, ptr));
}
#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_loader.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/assets/native_loader.hpp>
#include <zombye/audio/sound_collection.hpp>

zombye::sound_collection::sound_collection() {
}

Mix_Chunk* zombye::sound_collection::get(std::string name) {
    if(sounds_.find(name) != sounds_.end()) {
        return sounds_[name].get();
    }

    return nullptr;
}

void zombye::sound_collection::add(std::string name, std::string asset_path) {
    static zombye::asset_manager manager;

    auto asset = manager.load(asset_path);

    auto raw = zombye::get_raw_from_asset(asset.get());

    auto ptr = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV_RW(raw, 1), Mix_FreeChunk);

    if(!ptr) {
        zombye::log(LOG_ERROR, "Could not read sound " + std::string(Mix_GetError()));
    }

    sounds_.insert(std::make_pair(name, ptr));
}

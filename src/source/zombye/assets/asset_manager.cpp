#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_loader.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/assets/native_loader.hpp>
#include <zombye/utils/logger.hpp>

zombye::asset_manager::asset_manager() {
    // create asset loaders
    loaders_.emplace_back(new native_loader("assets"));
}

std::shared_ptr<zombye::asset> zombye::asset_manager::load(std::string path) const {
    for(auto &loader : loaders_) {
        auto ptr = loader->load(path);

        if(ptr) {
            return ptr;
        }
    }

    zombye::log(LOG_WARNING, "Could not find asset: " + path);

    return nullptr;
}

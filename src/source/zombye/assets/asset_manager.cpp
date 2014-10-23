#include <zombye/assets/asset_manager.hpp>

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

    return nullptr;
}
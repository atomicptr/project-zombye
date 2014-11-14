#include <zombye/assets/asset_manager.hpp>
#include <zombye/rendering/texture_manager.hpp>
#include <zombye/utils/load_dds.hpp>

namespace zombye {
    texture_ptr texture_manager::load_new(const std::string& name) {
        static zombye::asset_manager asset_manager{};
        auto asset = asset_manager.load(name);
        if (!asset) {
            return nullptr;
        }
        auto tex_image = gli::texture2D{gli::load_dds(asset->content())};
        if (tex_image.empty()) {
            return nullptr;
        }
        return std::make_shared<const texture>(tex_image);
    }
}
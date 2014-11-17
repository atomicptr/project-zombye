#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/texture_manager.hpp>
#include <zombye/utils/load_dds.hpp>

namespace zombye {
    texture_manager::texture_manager(game& game)
    : game_{game} { }

    texture_ptr texture_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
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
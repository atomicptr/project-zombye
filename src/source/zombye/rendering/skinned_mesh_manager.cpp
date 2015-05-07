#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/skinned_mesh_manager.hpp>

namespace zombye {
    skinned_mesh_manager::skinned_mesh_manager(game& game) noexcept
    : game_{game} { }

    skinned_mesh_ptr skinned_mesh_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const skinned_mesh>(game_.rendering_system(), asset->content(), name);
    }
}

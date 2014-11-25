#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/rigged_mesh_manager.hpp>

namespace zombye {
    rigged_mesh_manager::rigged_mesh_manager(game& game) noexcept
    : game_{game} { }

    rigged_mesh_ptr rigged_mesh_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const rigged_mesh>(game_.rendering_system(), asset->content());
    }
}
#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/physics/collision_mesh_manager.hpp>

namespace zombye {
    collision_mesh_manager::collision_mesh_manager(game& game) noexcept
    : game_{game} { }

    collision_mesh_ptr collision_mesh_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const collision_mesh>(*game_.physics(), asset->content());
    }
}

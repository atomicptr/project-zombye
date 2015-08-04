#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/rendering/mesh_manager.hpp>
#include <zombye/rendering/rendering_system.hpp>

namespace zombye {
    mesh_manager::mesh_manager(game& game, rendering_system& rendering_system) noexcept
    : game_{game}, rendering_system_{rendering_system} { }

    mesh_ptr mesh_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const mesh>(rendering_system_, asset->content(), name);
    }
}

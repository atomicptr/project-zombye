#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/mesh_manager.hpp>

namespace zombye {
    mesh_manager::mesh_manager(game& game) noexcept
    : game_{game} { }

    mesh_ptr mesh_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const mesh>(game_.rendering_system(), asset->content(), name);
    }
}

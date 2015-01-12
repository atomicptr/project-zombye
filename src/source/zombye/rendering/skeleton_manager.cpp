#include <zombye/core/game.hpp>
#include <zombye/rendering/skeleton_manager.hpp>

namespace zombye {
    skeleton_manager::skeleton_manager(game& game) noexcept
    : game_{game} { }

    skeleton_ptr skeleton_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const skeleton>(game_.rendering_system(), asset->content());
    }
}

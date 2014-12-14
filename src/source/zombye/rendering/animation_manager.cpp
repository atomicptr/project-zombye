#include <zombye/core/game.hpp>
#include <zombye/rendering/animation_manager.hpp>

namespace zombye {
    animation_manager::animation_manager(game& game) noexcept
    : game_{game} { }

    animation_ptr animation_manager::load_new(const std::string& name) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const animation>(game_.rendering_system(), asset->content());
    }
}
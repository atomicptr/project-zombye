#include <zombye/core/game.hpp>
#include <zombye/rendering/shader_manager.hpp>

namespace zombye {
    shader_manager::shader_manager(game& game) noexcept
    : game_{game} { }

    shader_ptr shader_manager::load_new(const std::string& name, GLenum type) {
        auto asset = game_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }

        auto& content = asset->content();
        auto source = std::string{content.begin(), content.end()};
        return std::make_shared<const shader>(name, type, source);
    }
}

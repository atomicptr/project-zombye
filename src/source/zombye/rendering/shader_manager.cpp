#include <zombye/assets/asset_manager.hpp>
#include <zombye/rendering/shader_manager.hpp>

namespace zombye {
    shader_ptr shader_manager::load_new(const std::string& name, GLenum type) {
        static zombye::asset_manager asset_manager{};
        auto asset = asset_manager.load(name);
        if (!asset) {
            return nullptr;
        }
        auto& content = asset->content();
        auto source = std::string{content.begin(), content.end()};
        return std::make_shared<shader>(name, type, source);
    }
}
#include <zombye/assets/asset_manager.hpp>
#include <zombye/rendering/mesh_manager.hpp>

namespace zombye {
    mesh_manager::mesh_manager(rendering_system& rendering_system) noexcept
    : rendering_system_{rendering_system} { }

    mesh_ptr mesh_manager::load_new(const std::string& name) {
        static zombye::asset_manager asset_manager{};
        auto asset = asset_manager.load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const mesh>(rendering_system_, asset->content());
    }
}
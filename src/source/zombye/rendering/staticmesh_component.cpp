#include <zombye/core/game.hpp>
#include <zombye/rendering/staticmesh_component.hpp>

namespace zombye {
    void staticmesh_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &staticmesh_component::set_mesh);
        register_property<std::string>("diffuse", nullptr, &staticmesh_component::set_diffuse_texture);
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner) noexcept
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner, const std::string& mesh,
    const std::string& diffuse_texture)
    : reflective{game, owner} {
        auto& rendering_system = game.rendering_system();
        rendering_system.register_component(this);
        mesh_ = rendering_system.mesh_manager().load(mesh);
        if (!mesh_) {
            throw std::runtime_error("could not load mesh from file " + mesh);
        }
        diffuse_texture_ = rendering_system.texture_manager().load(diffuse_texture);
        if (!diffuse_texture_) {
            throw std::runtime_error("coudl not load texture from file " + diffuse_texture);
        }
    }

    staticmesh_component::~staticmesh_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void staticmesh_component::set_mesh(const std::string& name) {
        auto tmp = game_.rendering_system().mesh_manager().load(name);
        if (tmp) {
            mesh_ = tmp;
        }
    }

    void staticmesh_component::set_diffuse_texture(const std::string& name) {
        auto tmp = game_.rendering_system().texture_manager().load(name);
        if (tmp) {
            diffuse_texture_ = tmp;
        }
    }
}
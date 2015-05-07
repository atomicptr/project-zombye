#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    staticmesh_component::staticmesh_component(game& game, entity& owner, const std::string& mesh)
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
        load(mesh);
    }

    staticmesh_component::~staticmesh_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void staticmesh_component::draw() const noexcept {
        mesh_->draw();
    }

    void staticmesh_component::load(const std::string& mesh) {
        auto& rendering_system = game_.rendering_system();

        mesh_ = rendering_system.mesh_manager().load(mesh);
        if (!mesh_) {
            log(LOG_FATAL, "could not load mesh from file " + mesh);
        }
    }

    staticmesh_component::staticmesh_component(game& game, entity& owner)
    : reflective(game, owner) {
        game_.rendering_system().register_component(this);
    }

    void staticmesh_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &staticmesh_component::load);
    }
}

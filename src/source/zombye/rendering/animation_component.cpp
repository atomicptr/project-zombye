#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    animation_component::animation_component(game& game, entity& owner, const std::string& mesh, const std::string& skeleton)
    : reflective{game, owner} {
        game_.rendering_system().register_component(this);
        load(mesh);
        load_skeleton(skeleton);
    }

    animation_component::~animation_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void animation_component::draw() const noexcept {
        mesh_->draw();
    }

    void animation_component::load(const std::string& mesh) {
        auto& rendering_system = game_.rendering_system();

        mesh_ = rendering_system.skinned_mesh_manager().load(mesh);
        if (!mesh_) {
            log(LOG_FATAL, "could not load mesh from file " + mesh);
        }
    }

    void animation_component::load_skeleton(const std::string& skeleton) {
        skeleton_ = game_.rendering_system().skeleton_manager().load(skeleton);
        if (!skeleton_) {
            log(LOG_FATAL, "could not load skeleton " + skeleton);
        }
    }

    animation_component::animation_component(game& game, entity& owner)
    : reflective(game, owner) {
        game_.rendering_system().register_component(this);
    }

    void animation_component::register_reflection() {
        register_property<std::string>("mesh", nullptr, &animation_component::load);
        register_property<std::string>("skeleton", nullptr, &animation_component::load_skeleton);
    }
}

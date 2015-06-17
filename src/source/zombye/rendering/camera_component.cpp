#include <glm/gtc/matrix_transform.hpp>

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    camera_component::camera_component(game& game, entity& owner, const glm::mat4& projection) noexcept
    : reflective{game, owner}, projection_{projection} {
        game_.rendering_system().register_component(this);
    }

    camera_component::camera_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, projection_{glm::mat4{1.f}} {}

    camera_component::~camera_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    void camera_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<camera_component>("camera_component");

        scripting_system.register_member_function("entity_impl",
            "camera_component& add_camera_component(const glm::mat4& in)",
            +[](entity& owner, const glm::mat4& projection) -> camera_component& {
                return owner.emplace<camera_component>(projection);
            });
        scripting_system.register_member_function("entity_impl", "camera_component@ get_camera_component()",
            +[](entity& owner) { return owner.component<camera_component>(); });
    }
}

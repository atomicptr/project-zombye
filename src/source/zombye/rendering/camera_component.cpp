#include <glm/gtc/matrix_transform.hpp>

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/rendering_system.hpp>

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
}

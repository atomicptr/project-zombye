#include <glm/gtc/matrix_transform.hpp>

#include <zombye/core/game.hpp>
#include <zombye/rendering/camera_component.hpp>

namespace zombye {
    camera_component::camera_component(game& game, entity& owner, const glm::vec3& look_at,
    const glm::vec3& up) noexcept
    : component{game, owner}, look_at_{look_at}, up_{up} {
        game_.rendering_system().register_component(this);
    }

    camera_component::~camera_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    glm::mat4 camera_component::transform() const {
        auto pos = owner_.position();
        auto position = glm::vec3{pos.x, pos.y, pos.z};
        return glm::lookAt(position, look_at_, up_);
    }
}
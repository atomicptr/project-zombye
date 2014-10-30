#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>

namespace zombye {
    unsigned long entity::next_id_ = 0;

    entity::entity(game& game, glm::vec3 position, glm::quat rotation, glm::vec3 scalation) noexcept
    : game_(game), id_(++next_id_), position_(position), rotation_(rotation), scalation_(scalation) { }

    glm::mat4 entity::transform() const {
        glm::mat4 transform = glm::toMat4(rotation_);
        transform[3].x = position_.x;
        transform[3].y = position_.y;
        transform[3].z = position_.z;
        return transform;
    }
}

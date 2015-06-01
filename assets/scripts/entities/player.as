#include "entity.as"

class player : entity {
    player(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale,
        const string& in mesh, const string& in skeleton) {
        super(position, rotation, scale);
        impl_.add_animation_component(mesh, skeleton);
    }
}

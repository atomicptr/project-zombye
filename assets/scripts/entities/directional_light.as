#include "entity.as"

class directional_light : entity {
    directional_light(const glm::vec3& in direction, const glm::vec3& in color = glm::vec3(0), float energy = 1.0) {
        super(direction, glm::quat(0, glm::vec3(0)), glm::vec3(1));
        impl_.add_directional_light_component(color, energy);
    }
}

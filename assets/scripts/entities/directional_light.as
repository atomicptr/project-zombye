#include "entity.as"

class directional_light : entity {
    directional_light(const glm::vec3& in direction, const glm::vec3& in color = glm::vec3(0), float energy = 1.0, bool shadow = false) {
        super(direction, glm::quat(0, glm::vec3(0)), glm::vec3(1));
        impl_.add_directional_light_component(color, energy);
        if (shadow) {
            impl_.add_shadow_component(glm::ortho(-30, 30, -30, 30, -30, 30));
        }
    }
}

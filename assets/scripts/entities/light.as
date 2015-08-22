#include "entity.as"

class light : entity {
    light(const glm::vec3& in position, const glm::vec3& in color, float radius, float exponent = 0.75f) {
        super(position, glm::quat(0, glm::vec3(0)), glm::vec3(1));
        impl_.add_light_component(color, radius, exponent);
    }
}

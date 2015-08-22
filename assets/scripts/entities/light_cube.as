#include "entity.as"

namespace light_cube {
    uint color_number = 0;
    array<glm::vec3> colors = {
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(1.0, 0.2, 0.1),
        glm::vec3(0.1, 0.8, 0.2),
        glm::vec3(0.2, 0.1, 0.8)
    };
}

class light_cube : entity {
    light_cube(const glm::vec3& in position, const glm::quat& in rotation) {
        super(position, rotation, glm::vec3(1));
        impl_.add_staticmesh_component("meshes/light_cube.msh");
        impl_.add_physics_component(box_shape(glm::vec3(0.25, 0.25, 0.25)), false);
        impl_.add_light_component(light_cube::colors[light_cube::color_number], 2, 1.75);
        light_cube::color_number = (light_cube::color_number + 1) % 4;
    }
}

#include "entity.as"

class light_cube : entity {
    light_cube(const glm::vec3& in position, const glm::quat& in rotation) {
        super(position, rotation, glm::vec3(1));
        impl_.add_staticmesh_component("meshes/light_cube.msh");
        impl_.add_physics_component(box_shape(glm::vec3(0.25, 0.25, 0.25)), false);
        impl_.add_light_component(glm::vec3(1), 5);
    }
}

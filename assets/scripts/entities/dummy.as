#include "entity.as"

class dummy : entity {
    dummy(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale, const string& in mesh) {
        super(position, rotation, scale);
        impl_.add_staticmesh_component(mesh);
        impl_.add_physics_component(box_shape(glm::vec3(1.2)), false);
    }
}

#include "entity.as"

class plane : entity {
    plane(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale) {
        super(position, rotation, scale);
        impl_.add_staticmesh_component("meshes/plane.msh");
        impl_.add_physics_component(triangle_mesh_shape("meshes/plane.col"), true);
    }
}

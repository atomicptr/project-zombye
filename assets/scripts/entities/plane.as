#include "entity.as"

class plane2 : entity {
    plane2(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale) {
        super(position, rotation, scale);
        impl_.add_staticmesh_component("meshes/brick_plane.msh");
        impl_.add_physics_component(box_shape(glm::vec3(scale.x, 0.0, scale.z)), true);
    }
}

class plane : entity {
    plane(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale) {
        super(position, rotation, scale);
        impl_.add_staticmesh_component("meshes/plane.msh");
        impl_.add_physics_component(triangle_mesh_shape("meshes/plane.col"), true);
    }
}

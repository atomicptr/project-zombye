#include "entity.as"

class skybox : entity {
    skybox(const glm::vec3& in scale = glm::vec3(100)) {
        super(glm::vec3(0, -80, 0), glm::quat(glm::radians(0), glm::vec3(0)), scale);
        impl_.add_staticmesh_component("meshes/skybox.msh");
        impl_.add_no_occluder_component();
    }
}

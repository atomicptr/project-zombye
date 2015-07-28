#include "entity.as"

class camera : entity {
    camera(const glm::vec3& in position, const glm::quat& in rotation) {
        super(position, rotation, glm::vec3(1.f));
        impl_.add_camera_component(glm::radians(45), width(), height(), 0.01f, 200.0f);
        impl_.add_camera_follow_component();
    }

    void activate() {
        activate_camera(impl_.id());
    }

    void follow(uint64 id) {
        impl_.get_camera_follow_component().target(id);
    }
}

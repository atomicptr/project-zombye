#include "entity.as"

class player : entity {
    player(const glm::vec3& in position, const glm::quat& in rotation, const glm::vec3& in scale,
        const string& in mesh, const string& in skeleton) {
        super(position, rotation, scale);
        impl_.add_animation_component(mesh, skeleton);
        //impl_.add_physics_component(box_shape(glm::vec3(0.6, 1, 0.3)), false);
        auto@ sc = impl_.add_state_component();
        sc.emplace("run", "scripts/character_states/run.as");
    }
}

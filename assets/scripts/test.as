#include "../assets/scripts/entities/dummy.as"
#include "../assets/scripts/entities/light.as"
#include "../assets/scripts/entities/player.as"
#include "../assets/scripts/entities/plane.as"

void main() {
    plane pl(glm::vec3(0), glm::quat(glm::radians(0), glm::vec3(0, 1, 0)), glm::vec3(1));
    dummy d(glm::vec3(0, 1, -2), glm::quat(glm::radians(45), glm::vec3(0, 1, 0)), glm::vec3(0.5f), "meshes/dummy.msh");
    light l(glm::vec3(0.f, 4.f, 0.f), glm::vec3(1), 10);
    player p(glm::vec3(0), glm::quat(glm::radians(0), glm::vec3(0, 1, 0)), glm::vec3(1), "meshes/human.msh", "anims/human.skl");
    auto anim = p.impl().get_animation_component();
    anim.play_ani("walk");
    anim.blend_ani("run");
    d.impl().get_physics_component().apply_central_impulse(glm::vec3(0, 5, 5));
}

#include "../assets/scripts/entities/light.as"
#include "../assets/scripts/entities/player.as"

void main() {
    light l(glm::vec3(-5.f, 20.f, 10.f), glm::vec3(1), 100);
    player p(glm::vec3(0), glm::quat(glm::radians(0), glm::vec3(0)), glm::vec3(1), "meshes/human.msh", "anims/human.skl");
    auto anim = p.impl().get_animation_component();
    anim.play_ani("walk");
    anim.blend_ani("run");
}

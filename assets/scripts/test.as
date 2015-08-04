#include "../assets/scripts/entities/camera.as"
#include "../assets/scripts/entities/dummy.as"
#include "../assets/scripts/entities/directional_light.as"
#include "../assets/scripts/entities/light.as"
#include "../assets/scripts/entities/player.as"
#include "../assets/scripts/entities/plane.as"
#include "../assets/scripts/entities/skybox.as"

void main() {
    camera c(glm::vec3(-10, 4, 0), glm::quat(glm::radians(-15), glm::vec3(1, 0, 0)));
    c.activate();
    plane pl(glm::vec3(0, 1, 0), glm::quat(glm::radians(0), glm::vec3(1, 0, 0)), glm::vec3(1));
    dummy d(glm::vec3(4, 4, 0), glm::quat(glm::radians(45), glm::vec3(0, 1, 0)), glm::vec3(1.f), "meshes/dummy.msh");
    light l(glm::vec3(-4.f, 4.f, 2.f), glm::vec3(1), 20);
    light l2(glm::vec3(10.f, 2.f, -2.f), glm::vec3(0.f, 0.3f, 1.0f), 20);
    player p(glm::vec3(-3.f, 2.f, -14.f), glm::quat(glm::radians(90), glm::vec3(0, 1, 0)), glm::vec3(1), "meshes/human.msh", "anims/human.skl");
    player_id = p.id();
    directional_light dl(glm::vec3(1, 0.3, 1), glm::vec3(1), 1, true);
    c.follow(p.id());
    c.impl().get_camera_follow_component().initial_position(glm::vec3(0,0,0));
}

#include <zombye/gameplay/states/menu_state.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/physics/physics_component.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <btBulletDynamicsCommon.h>

using namespace std::string_literals;

zombye::menu_state::menu_state(zombye::state_machine *sm) : sm_(sm) {

}

void zombye::menu_state::enter() {
    zombye::log("enter menu state");
    auto& em = sm_->get_game()->entity_manager();
    auto& camera = em.emplace(glm::vec3{-2.f, 2.f, -3.f}, glm::quat{0.f, 0.f, 0.f, 1.f}, glm::vec3{1.f});
    camera.emplace<camera_component>(glm::vec3{}, glm::vec3{0.f, 1.f, 0.f});
    sm_->get_game()->rendering_system().activate_camera(camera.id());

    auto& anim = em.emplace("animated", glm::vec3{0.f}, glm::quat{0.f, 0.f , 0.f , 1.f}, glm::vec3{1.f});
    //auto& dummy = em.emplace("dummy", glm::vec3{1, 20, 0}, glm::quat{0, 0, 0, 1}, glm::vec3{1});
    //auto& dummy2 = em.emplace("dummy", glm::vec3{0, -1, 0}, glm::quat{0, 0, 0, 1}, glm::vec3{1});

    //dummy.emplace<physics_component>(new btBoxShape(btVector3(1, 1, 1)));
    //dummy2.emplace<physics_component>(new btBoxShape(btVector3(1, 1, 1)), true);
    auto& light = em.emplace("point_light", glm::vec3{7.f, 2.f, -4.f},
        glm::quat{0.f, 0.f , 0.f , 1.f}, glm::vec3{1.f});
    auto& light2 = em.emplace("point_light", glm::vec3{0.f, 5.f, -2.f},
        glm::quat{0.f, 0.f , 0.f , 1.f}, glm::vec3{1.f});
    light2.component<light_component>()->set_color(glm::vec3{0.5f, 1.0f, 0.5f});
    auto& light3 = em.emplace("point_light", glm::vec3{1.f, 3.f, -4.f},
        glm::quat{0.f, 0.f , 0.f , 1.f}, glm::vec3{1.f});
    light3.component<light_component>()->set_color(glm::vec3{0.f, 0.f, 1.0f});
    //auto& sphere = em.emplace("sphere", glm::vec3{0.f}, glm::quat{0.f, 0.f, 0.f, 1.f}, glm::vec3{1.f});
}

void zombye::menu_state::leave() {
    zombye::log("leave menu state");
}

void zombye::menu_state::update(float delta_time) {

}

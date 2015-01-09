#include <zombye/gameplay/states/menu_state.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/physics/physics_component.hpp>

#include <zombye/physics/shapes.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std::string_literals;

zombye::menu_state::menu_state(zombye::state_machine *sm) : sm_(sm) {

}

void zombye::menu_state::enter() {
    zombye::log("enter menu state");

    auto& em = sm_->get_game()->entity_manager();
    auto& camera = em.emplace(glm::vec3{-2.f, 2.f, -3.f}, glm::quat{0.f, 0.f, 0.f, 1.f}, glm::vec3{1.f});
    camera.emplace<camera_component>(glm::vec3{}, glm::vec3{0.f, 1.f, 0.f});
    sm_->get_game()->rendering_system().activate_camera(camera.id());

    auto& dummy = sm_->get_game()->entity_manager().emplace("suzanne", glm::vec3{1.3, 20, 0}, glm::quat{0, 0, 0, 1}, glm::vec3{1});
    auto& dummy2 = sm_->get_game()->entity_manager().emplace("suzanne", glm::vec3{0, -3, 0}, glm::quat{0, 0, 0, 1}, glm::vec3{1});

    dummy.emplace<physics_component>(new sphere_shape(2));
    dummy2.emplace<physics_component>(new convex_hull_shape(nullptr), true);

    sm_->get_game()->entity_manager().emplace("light", glm::vec3{5.f, 30.f, 5.f}, glm::quat{0.f, 0.f, 1.f, 0.f}, glm::vec3{1.f});
}

void zombye::menu_state::leave() {
    zombye::log("leave menu state");
}

void zombye::menu_state::update(float delta_time) {

}

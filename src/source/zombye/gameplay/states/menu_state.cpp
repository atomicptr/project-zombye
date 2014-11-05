#include <string>

#include <zombye/gameplay/states/menu_state.hpp>
#include <zombye/ecs/staticmesh_component.hpp>
#include <zombye/ecs/health_component.hpp>

using namespace std::string_literals;

zombye::menu_state::menu_state(zombye::state_machine *sm) : sm_(sm) {

}

void zombye::menu_state::enter() {
    zombye::log("enter menu state");
    auto& ent = sm_->game()->entity_manager().emplace(glm::vec3(), glm::quat(), glm::vec3());
    ent.emplace("health_component"s, 10.f, 15.f);
    ent.emplace("staticmesh_component"s, "models/goblin.msh"s);
    log(ent.component<zombye::staticmesh_component>()->file());
    log(std::to_string(ent.component<zombye::health_component>()->health()));
}

void zombye::menu_state::leave() {
    zombye::log("leave menu state");
}

void zombye::menu_state::update(float delta_time) {
    sm_->use(GAME_STATE_PLAY);
}
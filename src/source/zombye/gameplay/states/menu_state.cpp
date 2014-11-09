#include <string>

#include <zombye/gameplay/states/menu_state.hpp>
#include <zombye/ecs/staticmesh_component.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/test_component.hpp>

using namespace std::string_literals;

zombye::menu_state::menu_state(zombye::state_machine *sm) : sm_(sm) {

}

void zombye::menu_state::enter() {
    zombye::log("enter menu state");
    auto& ent = sm_->game()->entity_manager().emplace("goblin", glm::vec3(), glm::quat(), glm::vec3());
    log(std::to_string(ent.component<zombye::health_component>()->health()));
    log(std::to_string(ent.component<zombye::health_component>()->max_health()));
    log(ent.component<zombye::staticmesh_component>()->file());
    auto test_component = ent.component<zombye::test_component>();
    log(std::to_string(test_component->boolean()));
    log(std::to_string(test_component->integer()));
    std::string s = "";
    for (auto i = 0; i < 2; i++) {
        s += std::to_string(test_component->ivec2()[i]) + " ";
    }
    log(s);

    std::string s2 = "";
    for (auto i = 0; i < 4; i++) {
        s2 += std::to_string(test_component->quat()[i]) + " ";
    }
    log(s2);
    log(test_component->string());
    auto& ent2 = sm_->game()->entity_manager().emplace("wolf", glm::vec3(), glm::quat(), glm::vec3());
    log(std::to_string(ent2.component<zombye::health_component>()->health()));
    log(std::to_string(ent2.component<zombye::health_component>()->max_health()));
    log(ent2.component<zombye::staticmesh_component>()->file());
}

void zombye::menu_state::leave() {
    zombye::log("leave menu state");
}

void zombye::menu_state::update(float delta_time) {
    sm_->use(GAME_STATE_PLAY);
}
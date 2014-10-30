#include <iostream>
#include <string>

#include <zombye/core/game.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/ecs/rtti_manager.hpp>

int main() {
    zombye::game game("project-zombye", 800, 600);

    zombye::entity ent(game, glm::vec3(), glm::quat(), glm::vec3());
    auto& hc = ent.emplace("health_component", 10.0f, 20.0f);
    std::cout << ent.component<zombye::health_component>()->health() << std::endl;
    std::cout << ent.component<zombye::health_component>()->max_health() << std::endl;
    game.run();

    return 0;
}

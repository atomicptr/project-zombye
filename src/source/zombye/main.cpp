#include <iostream>
#include <string>

#include <zombye/core/game.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/staticmesh_component.hpp>
#include <zombye/ecs/entity.hpp>

int main() {
    zombye::game game("project-zombye", 800, 600);

    zombye::entity ent(game, glm::vec3(), glm::quat(), glm::vec3());
    auto& hc = ent.emplace("health_component", 10.0f, 20.0f);
    auto& sc = ent.emplace("staticmesh_component", "assets/models/goblin.msh");
    std::cout << ent.component<zombye::health_component>()->health() << std::endl;
    std::cout << ent.component<zombye::health_component>()->max_health() << std::endl;
    ent.emplace("health_component", 10.0f, 20.0f);
    ent.erase<zombye::health_component>();
    ent.emplace<zombye::health_component>(10.0f, 20.0f);
    ent.emplace("staticmesh_component", "assets/models/goblin.msh");
    ent.emplace<zombye::staticmesh_component>("assets/models/goblin.msh");
    ent.erase<zombye::health_component>();
    ent.erase<zombye::health_component>();
    std::cout << ent.component<zombye::staticmesh_component>()->file() << std::endl;
    game.run();

    return 0;
}

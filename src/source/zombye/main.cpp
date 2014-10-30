#include <iostream>
#include <string>

#include <zombye/core/game.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/entity.hpp>

int main() {
    zombye::game game("project-zombye", 800, 600);

    zombye::entity ent(game, glm::vec3(), glm::quat(), glm::vec3());
    zombye::health_component* hc = zombye::health_component::create(game, ent);
    std::cout << zombye::health_component::type_rtti()->properties().size() << std::endl;
    std::cout << hc->type_rtti()->properties().size() << std::endl;
    std::cout << hc->type_rtti()->properties()[0]->name() << std::endl;
    std::cout << hc->type_rtti()->properties()[1]->name() << std::endl;
    auto health = std::static_pointer_cast<zombye::typed_property<float>>(hc->type_rtti()->properties()[0]);
    health->value(hc, 50);
    std::cout << health->value(hc) << std::endl;
    std::cout << hc->get_health() << std::endl;

    game.run();

    return 0;
}

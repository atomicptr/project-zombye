#include <iostream>
#include <string>

#include <zombye/core/game.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/staticmesh_component.hpp>
#include <zombye/ecs/entity.hpp>

int main() {
    zombye::game game("project-zombye", 800, 600);

    game.run();

    return 0;
}

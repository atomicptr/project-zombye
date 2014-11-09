#include <stdexcept>

#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>

namespace zombye {
    component::component(game& game, entity& owner) noexcept
    : reflective(game, owner), game_(game), owner_(owner) { }

    component::~component() noexcept { }

    component* component::create(game& game, entity& owner) {
        throw std::bad_alloc();
    }
}

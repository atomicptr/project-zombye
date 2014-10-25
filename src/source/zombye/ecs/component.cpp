#include <zombye/core/game.hpp>
#include <zombye/ecs/component.hpp>
#include <zombye/ecs/entity.hpp>

namespace zombye {
    component::component(game& game, entity& owner) noexcept
    : game_(game), owner_(owner) { }

    component::~component() noexcept { }
}

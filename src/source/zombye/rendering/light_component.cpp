#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/rendering_system.hpp>

namespace zombye {
    light_component::light_component(game& game, entity& owner, const glm::vec3& color, const glm::vec3& specular_color, float distance) noexcept
    : reflective{game, owner}, color_{color}, specular_color_{specular_color}, distance_{distance} {
        game_.rendering_system().register_component(this);
    }

    light_component::~light_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    light_component::light_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, color_{1.f}, specular_color_{1.f}, distance_{30.f} {
        game_.rendering_system().register_component(this);
    }

    void light_component::register_reflection() {
        register_property<glm::vec3>("color", &light_component::color, &light_component::set_color);
        register_property<glm::vec3>("specular_color", &light_component::specular_color, &light_component::set_specular_color);
        register_property<float>("distance", &light_component::distance, &light_component::set_distance);
    }
}

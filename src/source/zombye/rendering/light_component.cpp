#include <zombye/core/game.hpp>
#include <zombye/rendering/light_component.hpp>

namespace zombye {
    void light_component::register_reflection() {
        register_property<glm::vec3>("color", &light_component::color, &light_component::set_color);
        register_property<float>("intensity", &light_component::intensity, &light_component::set_intensity);
    }

    light_component::light_component(game& game, entity& owner)
    : reflective{game, owner}, color_{0.f}, intensity_{0.f} {
        game_.rendering_system().register_component(this);
    }

    light_component::light_component(game& game, entity& owner, const glm::vec3& color, float intensity)
    : reflective{game, owner}, color_{color}, intensity_{intensity} {
        game_.rendering_system().register_component(this);
    }

    light_component::~light_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }
}
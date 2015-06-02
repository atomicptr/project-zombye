#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>

#include <zombye/rendering/directional_light_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    directional_light_component::directional_light_component(game& game, entity& owner, const glm::vec3& color, float energy) noexcept
    : reflective{game, owner}, color_{color}, energy_{energy} {
        game_.rendering_system().register_component(this);
    }

    directional_light_component::~directional_light_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    directional_light_component::directional_light_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, color_{1.f}, energy_{30.f} {
        game_.rendering_system().register_component(this);
    }

    void directional_light_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<directional_light_component>("directional_light_component");

        scripting_system.register_member_function("directional_light_component", "const glm::vec3& color() const",
            +[](directional_light_component& component) -> const glm::vec3& { return component.color(); });
        scripting_system.register_member_function("directional_light_component", "void color(const glm::vec3& in)",
            +[](directional_light_component& component, const glm::vec3& color) { component.color(color); });
        scripting_system.register_member_function("directional_light_component", "float energy() const",
            +[](directional_light_component& component) { return component.energy(); });
        scripting_system.register_member_function("directional_light_component", "void energy(float)",
            +[](directional_light_component& component, float energy) { component.energy(energy); });

        scripting_system.register_member_function("entity_impl",
            "directional_light_component& add_directional_light_component(const glm::vec3& in, float)",
            +[](entity& owner, const glm::vec3& color, float energy) -> directional_light_component& {
                return owner.emplace<directional_light_component>(color, energy);
            });
        scripting_system.register_member_function("entity_impl", "directional_light_component@ get_directional_light_component()",
            +[](entity& owner) { return owner.component<directional_light_component>(); });
    }

    void directional_light_component::register_reflection() {

    }
}

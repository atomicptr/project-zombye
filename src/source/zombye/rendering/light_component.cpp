#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    light_component::light_component(game& game, entity& owner, const glm::vec3& color, const glm::vec3& specular_color, float distance, float exponent) noexcept
    : reflective{game, owner}, color_{color}, specular_color_{specular_color}, distance_{distance}, exponent_{exponent} {
        game_.rendering_system().register_component(this);
    }

    light_component::~light_component() noexcept {
        game_.rendering_system().unregister_component(this);
    }

    light_component::light_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, color_{1.f}, specular_color_{1.f}, distance_{30.f}, exponent_{0.5f} {
        game_.rendering_system().register_component(this);
    }

    void light_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<light_component>("light_component");

        scripting_system.register_member_function("light_component", "const glm::vec3& color() const",
            +[](light_component& component) -> const glm::vec3& { return component.color(); });
        scripting_system.register_member_function("light_component", "void color(const glm::vec3& in)",
            +[](light_component& component, const glm::vec3& color) { component.color(color); });
        scripting_system.register_member_function("light_component", "float radius() const",
            +[](light_component& component) { return component.distance(); });
        scripting_system.register_member_function("light_component", "void radius(float)",
            +[](light_component& component, float radius) { component.distance(radius); });
        scripting_system.register_member_function("light_component", "float exponent() const",
            +[](light_component& component) { return component.exponent(); });
        scripting_system.register_member_function("light_component", "void exponent(float)",
            +[](light_component& component, float exponent) { component.exponent(exponent); });

        scripting_system.register_member_function("entity_impl",
            "light_component& add_light_component(const glm::vec3& in, float radius, float exponent)",
            +[](entity& owner, const glm::vec3& color, float radius, float exponent) -> light_component& {
                return owner.emplace<light_component>(color, color, radius, exponent);
            });
        scripting_system.register_member_function("entity_impl", "light_component@ get_light_component()",
            +[](entity& owner) { return owner.component<light_component>(); });
    }

    void light_component::register_reflection() {

    }
}

#include <zombye/ecs/entity.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/shadow_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    shadow_component::shadow_component(game& game, entity& owner, const glm::mat4& projection) noexcept
    : reflective{game, owner}, projection_{projection} {
        game_.rendering_system().register_component(this);
    }

    shadow_component::~shadow_component() {
        game_.rendering_system().unregister_component(this);
    }

    void shadow_component::register_at_script_engine(game& game) {
        auto& scripting_system = game.scripting_system();

        scripting_system.register_type<shadow_component>("shadow_component");

        scripting_system.register_member_function("entity_impl",
            "shadow_component& add_shadow_component(const glm::mat4& in)",
            +[](entity& owner, const glm::mat4& projection) -> shadow_component& {
                return owner.emplace<shadow_component>(projection);
            });
        scripting_system.register_member_function("entity_impl", "shadow_component@ get_shadow_component()",
            +[](entity& owner) { return owner.component<shadow_component>(); });
    }

    shadow_component::shadow_component(game& game, entity& owner) noexcept
    : reflective{game, owner}, projection_{1.f} {
        game_.rendering_system().register_component(this);
    }
}

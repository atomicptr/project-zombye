#include <zombye/ecs/entity.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/no_occluder_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
	no_occluder_component::no_occluder_component(game& game, entity& owner)
	: reflective{game, owner} {}

	void no_occluder_component::register_at_script_engine(game& game) {
		auto& scripting_system = game.scripting_system();

		scripting_system.register_type<no_occluder_component>("no_occluder_component");

		scripting_system.register_member_function("entity_impl",
			"no_occluder_component& add_no_occluder_component()",
			+[](entity& owner) -> no_occluder_component& {
				return owner.emplace<no_occluder_component>();
			});
		scripting_system.register_member_function("entity_impl", "no_occluder_component@ get_no_occluder_component()",
			+[](entity& owner) { return owner.component<no_occluder_component>(); });
	}
}

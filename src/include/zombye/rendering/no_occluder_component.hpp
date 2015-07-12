#ifndef __ZOMBYE_NO_OCCLUDER_COMPONENT_HPP__
#define __ZOMBYE_NO_OCCLUDER_COMPONENT_HPP__

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
	class entity;
	class game;
}

namespace zombye {
	class no_occluder_component : public reflective<no_occluder_component, component> {
		friend class reflective<no_occluder_component, component>;

	public:
		no_occluder_component(game& game, entity& owner);
		~no_occluder_component() = default;

		static void register_at_script_engine(game& game);
	};
}

#endif

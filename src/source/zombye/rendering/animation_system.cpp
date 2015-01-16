#include <zombye/core/game.hpp>
#include <zombye/rendering/animation_system.hpp>
#include <zombye/utils/component_helper.hpp>

namespace zombye {
	animation_system::animation_system(game& game)
	: game_{game} { }

	void animation_system::update(float delta_time) {
		for (auto& ac : animation_components_) {
			ac->update(delta_time);
		}
	}

    void animation_system::register_component(animation_component* component) {
		animation_components_.emplace_back(component);
    }

    void animation_system::unregister_component(animation_component* component) {
		remove(animation_components_, component);    	
    }
}
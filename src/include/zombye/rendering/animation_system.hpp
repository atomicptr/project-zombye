#ifndef __ZOMBYE_ANIMATION_SYSTEM_HPP__
#define __ZOMBYE_ANIMATION_SYSTEM_HPP__

#include <vector>

namespace zombye {
	class animation_component;
	class game;
}

namespace zombye {
	class animation_system {
		friend class animation_component;

		game& game_;

		std::vector<animation_component*> animation_components_;

	public:
		animation_system(game& game);
		~animation_system() = default;

		animation_system(const animation_system& rhs) = delete;
		animation_system& operator=(const animation_system& rhs) = delete;

		animation_system(animation_system&& rhs) = delete;
		animation_system& operator=(animation_system&& rhs) = delete;

		void update(float delta_time);

	private:
        void register_component(animation_component* component);
        void unregister_component(animation_component* component);
	};
}

#endif

#ifndef __ZOMBYE_GAMEPLAY_SYSTEM_HPP__
#define __ZOMBYE_GAMEPLAY_SYSTEM_HPP__

#include <string>
#include <vector>

namespace zombye {
    class camera_follow_component;
    class state_component;
}

namespace zombye {
    class game;
    class state_machine;

    class gameplay_system {
        friend class camera_follow_component;
        friend class state_component;
    public:
        gameplay_system(game*);

        void use(std::string);
        void dispose_current();

        void update(float);

    private:
        void register_component(camera_follow_component* component);
        void unregister_component(camera_follow_component* component);
        void register_component(state_component* component);
        void unregister_component(state_component* component);

    private:
        std::unique_ptr<zombye::state_machine> sm_;
        std::vector<camera_follow_component*> camera_follow_components_;
        std::vector<state_component*> state_components_;

        void init_game_states();
    };
}

#endif

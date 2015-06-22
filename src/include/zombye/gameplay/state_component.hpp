#ifndef __ZOMBYE_STATE_COMPONENT_HPP__
#define __ZOMBYE_STATE_COMPONENT_HPP__

#include <string>
#include <unordered_map>

#include <angelscript.h>

#include <zombye/ecs/component.hpp>
#include <zombye/ecs/reflective.hpp>

namespace zombye {
    class entity;
    class game;
    class scripting_system;
}

namespace zombye {
    struct character_state {
        asIScriptFunction* enter;
        asIScriptFunction* update;
        asIScriptFunction* leave;
    };
}

namespace zombye {
    class state_component : public reflective<state_component, component> {
        friend class reflective<state_component, component>;

    private:
        scripting_system& scripting_system_;
        std::unordered_map<std::string, character_state> states_;
        character_state* current_state_;

    public:
        state_component(game& game, entity& owner);
        ~state_component();

        void emplace(const std::string& state_name, const std::string& file_name);
        void change_state(const std::string& state_name);
        void update(float delta_time);
    };
}

#endif

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity.hpp>
#include <zombye/gameplay/gameplay_system.hpp>
#include <zombye/gameplay/state_component.hpp>
#include <zombye/scripting/scripting_system.hpp>

namespace zombye {
    state_component::state_component(game& game, entity& owner)
    : reflective{game, owner}, scripting_system_{game_.scripting_system()}, current_state_{nullptr} {
        game_.gameplay()->register_component(this);
    }

    state_component::~state_component() {
        game_.gameplay()->unregister_component(this);
    }

    void state_component::emplace(const std::string& state_name, const std::string& file_name) {
        auto it = states_.find(state_name);
        if (it == states_.end()) {
            throw std::runtime_error("state " + state_name + " already exists in " + std::to_string(owner_.id()));
        }

        static auto state_prefix = std::to_string(owner_.id()) + "_";
        auto module_name = state_prefix + state_name;

        scripting_system_.begin_module(module_name);
        scripting_system_.load_script(file_name);
        scripting_system_.end_module();

        auto module = scripting_system_.script_engine().GetModule(module_name.c_str());
        if (!module) {
            throw std::runtime_error("no module named " + module_name);
        }

        auto enter_ptr = module->GetFunctionByDecl("void enter()");
        if (!enter_ptr) {
            throw std::runtime_error("no function callback for enter state in " + file_name);
        }

        auto update_ptr = module->GetFunctionByDecl("void update(float delta_time)");
        if (!update_ptr) {
            throw std::runtime_error("no function callback for update state in " + file_name);
        }

        auto leave_ptr = module->GetFunctionByDecl("void leave()");
        if (!leave_ptr) {
            throw std::runtime_error("no function callback for leave state in " + file_name);
        }

        states_.insert(std::make_pair(state_name, character_state{enter_ptr, update_ptr, leave_ptr}));
    }

    void state_component::change_state(const std::string& state_name) {
        auto it = states_.find(std::to_string(owner_.id()) + "_" + state_name);
        if (it == states_.end()) {
            throw std::runtime_error("entity " + std::to_string(owner_.id()) + " has no state " + state_name);
        }

        static auto& script_context = scripting_system_.script_context();

        if (current_state_) {
            script_context.Prepare(current_state_->leave);
            auto result = script_context.Execute();
            if (result != asEXECUTION_FINISHED) {
                if (result == asEXECUTION_EXCEPTION) {
                    throw std::runtime_error(script_context.GetExceptionString());
                }
            }
        }

        current_state_ = &(it->second);
        script_context.Prepare(current_state_->enter);
        auto result = script_context.Execute();
        if (result != asEXECUTION_FINISHED) {
            if (result == asEXECUTION_EXCEPTION) {
                throw std::runtime_error(script_context.GetExceptionString());
            }
        }
    }

    void state_component::update(float delta_time) {
        static auto& script_context = scripting_system_.script_context();

        if (current_state_) {
            script_context.Prepare(current_state_->update);
            script_context.SetArgFloat(0, delta_time);
            auto result = script_context.Execute();
            if (result != asEXECUTION_FINISHED) {
                if (result == asEXECUTION_EXCEPTION) {
                    throw std::runtime_error(script_context.GetExceptionString());
                }
            }
        }
    }
}

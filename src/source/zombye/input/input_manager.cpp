#include <json/json.h>

#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/input/button.hpp>
#include <zombye/input/input_manager.hpp>
#include <zombye/input/input_system.hpp>
#include <zombye/input/keyboard.hpp>
#include <zombye/scripting/scripting_system.hpp>

zombye::input_manager::input_manager(input_system *input) : input_(input) {
}

void zombye::input_manager::register_event(std::string event_name, zombye::button &btn) {
    btn.register_keydown_listener([event_name, that=this](button &b) {
        that->event_queue_.push(that->commands_.at(event_name));
    });
}

void zombye::input_manager::register_up_event(std::string event_name, zombye::button &btn) {
    btn.register_keyup_listener([event_name, that=this](button &b) {
        that->event_queue_.push(that->commands_.at(event_name));
    });
}

void zombye::input_manager::register_keyboard_event(std::string event_name, std::string key, bool continuous) {
    input_->keyboard()->register_keydown_listener(key, [event_name, that=this]() {
        that->event_queue_.push(that->commands_.at(event_name));
    }, continuous);
}

void zombye::input_manager::register_keyboard_up_event(std::string event_name, std::string key, bool continuous) {
    input_->keyboard()->register_keyup_listener(key, [event_name, that=this]() {
        that->event_queue_.push(that->commands_.at(event_name));
    }, continuous);
}

void zombye::input_manager::register_action(std::string event_name, std::function<void()> cmd) {
    commands_.insert(std::make_pair(event_name, cmd));
}

void zombye::input_manager::register_actions(game& game, const std::string& file_name) {
    auto& scripting_system = game.scripting_system();
    auto& script_engine = scripting_system.script_engine();

    scripting_system.begin_module(file_name);
    scripting_system.load_script(file_name);
    scripting_system.end_module();

    auto module = script_engine.GetModule(file_name.c_str());
    if (!module) {
        throw std::runtime_error("no module named " + file_name);
    }

    for (auto i = 0u; i < module->GetFunctionCount(); ++i) {
        auto func = module->GetFunctionByIndex(i);
        auto func_name = func->GetName();
        register_action(func_name,
            [func, &scripting_system]() {
                scripting_system.prepare(*func);
                scripting_system.exec();
            }
        );
    }
}

void zombye::input_manager::load_config(game& game, const std::string& file_name) {
    auto input_config = game.asset_manager().load(file_name);
    if (!input_config) {
        throw std::runtime_error("Could not load input config " + file_name);
    }

    Json::Reader reader;
    Json::Value root;
    auto& content = input_config->content();
    reader.parse(content.data(), content.data() + content.size(), root);

    for (auto it = root.begin(); it != root.end(); ++it) {
        auto action_base_name = it.key().asString();
        auto action_key = it->asString();

        auto action_begin_name = action_base_name + "_begin";
        auto command_it = commands_.find(action_begin_name);
        if (command_it == commands_.end()) {
            throw std::runtime_error("No action registered with name " + action_begin_name);
        }
        register_keyboard_event(action_begin_name, action_key, true);

        auto action_end_name = action_base_name + "_end";
        command_it = commands_.find(action_end_name);
        if (command_it == commands_.end()) {
            throw std::runtime_error("No action registered with name " + action_end_name);
        }
        register_keyboard_up_event(action_end_name, action_key, true);
    }
}

void zombye::input_manager::handle_input() {
    while(!event_queue_.empty()) {
        auto cmd = event_queue_.front();

        event_queue_.pop();

        cmd();
    }
}

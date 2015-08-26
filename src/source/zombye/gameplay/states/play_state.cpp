#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/gameplay/states/play_state.hpp>
#include <zombye/input/input_manager.hpp>
#include <zombye/input/input_system.hpp>
#include <zombye/input/joystick.hpp>
#include <zombye/input/mouse.hpp>
#include <zombye/physics/shapes/box_shape.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/scripting/scripting_system.hpp>
#include <zombye/utils/logger.hpp>
#include <zombye/utils/state_machine.hpp>

#include <zombye/physics/physics_system.hpp>

zombye::play_state::play_state(zombye::state_machine *sm) : sm_(sm) {
    auto input = sm->get_game()->input();
    input_ = input->create_manager();

    input_->register_actions(*sm->get_game(), "scripts/input/play_state.as");
    input_->load_config(*sm->get_game(), "config/input/play_state.json");
}

void zombye::play_state::enter() {
    zombye::log("enter play state");

    auto& game = *sm_->get_game();

    auto& scripting_system = game.scripting_system();
    scripting_system.begin_module("MyModule");
    scripting_system.load_script("scripts/test.as");
    scripting_system.end_module();
    scripting_system.exec("void main()", "MyModule");

    auto& em = game.entity_manager();

    auto player = em.resolve(6);
    auto box = em.resolve(3);

    auto physics = game.physics();

    physics->register_collision_begin_callback(box, player, [](auto box, auto player) {
        zombye::log("COLLISION BEGIN");
    });

    physics->register_collision_callback(box, player, [](auto box, auto player) {
        zombye::log("COLLISION");
    });

    physics->register_collision_end_callback(box, player, [](auto box, auto player) {
        zombye::log("COLLISION END");
    });
}

void zombye::play_state::leave() {
    zombye::log("leave play state");
}

void zombye::play_state::update(float delta_time) {
    input_->handle_input();
}

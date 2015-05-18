#include <cmath>

#include <glm/gtx/string_cast.hpp>
#include <SDL2/SDL.h>

#include <zombye/core/game.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/gameplay/states/play_state.hpp>
#include <zombye/gameplay/command.hpp>
#include <zombye/input/input_manager.hpp>
#include <zombye/input/input_system.hpp>
#include <zombye/input/joystick.hpp>
#include <zombye/input/mouse.hpp>
#include <zombye/physics/shapes/box_shape.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/utils/logger.hpp>
#include <zombye/utils/state_machine.hpp>

class test_command : public zombye::command {
public:
    test_command() {}

    void execute() {
        zombye::log("FIRE!!!! PENG PENG!!!");
    }
};

class switch_anim : public zombye::command {
private:
    zombye::game& game_;
    bool toggle_ = false;
public:
    switch_anim(zombye::game& game) : game_{game} {}

    void execute() {
        auto anim = game_.entity_manager().resolve(2);
        if (anim) {
            if (toggle_) {
                auto comp = anim->component<zombye::animation_component>();
                if (comp->is_playing("walk") && !comp->is_blending()) {
                    comp->change_state_blend("move");
                } else {
                    return;
                }
            } else {
                auto comp = anim->component<zombye::animation_component>();
                if (comp->is_playing("move") && !comp->is_blending()) {
                    comp->change_state_blend("walk");
                } else {
                    return;
                }
            }
            toggle_ = !toggle_;
        }
    }
};

class cam_forward : public zombye::command {
public:
    cam_forward(zombye::game& game) : renderer_{game.rendering_system()} {}

    void execute() {
        auto cam = renderer_.active_camera();
        if (cam) {
            auto& owner = cam->owner();
            auto pos = owner.position();
            auto look_at = cam->look_at();
            auto direction = glm::vec3{1.f, 0.f, 1.f};
            owner.position(pos - 0.3f * direction);
            cam->set_look_at(look_at - 0.3f * direction);
        }
    }
private:
    zombye::rendering_system& renderer_;
};

class cam_backward : public zombye::command {
public:
    cam_backward(zombye::game& game) : renderer_{game.rendering_system()} {}

    void execute() {
        auto cam = renderer_.active_camera();
        if (cam) {
            auto& owner = cam->owner();
            auto pos = owner.position();
            auto look_at = cam->look_at();
            auto direction = glm::vec3{1.f, 0.f, 1.f};
            owner.position(pos + 0.3f * direction);
            cam->set_look_at(look_at + 0.3f * direction);
        }
    }
private:
    zombye::rendering_system& renderer_;
};

class cam_left : public zombye::command {
public:
    cam_left(zombye::game& game) : renderer_{game.rendering_system()} {}

    void execute() {
        auto cam = renderer_.active_camera();
        if (cam) {
            auto& owner = cam->owner();
            auto pos = owner.position();
            auto look_at = cam->look_at();
            auto direction = glm::vec3{1.f, 0.f, -1.f};
            owner.position(pos - 0.3f * direction);
            cam->set_look_at(look_at - 0.3f * direction);
        }
    }
private:
    zombye::rendering_system& renderer_;
};

class cam_right : public zombye::command {
public:
    cam_right(zombye::game& game) : renderer_{game.rendering_system()} {}

    void execute() {
        auto cam = renderer_.active_camera();
        if (cam) {
            auto& owner = cam->owner();
            auto pos = owner.position();
            auto look_at = cam->look_at();
            auto direction = glm::vec3{1.f, 0.f, -1.f};
            owner.position(pos + 0.3f * direction);
            cam->set_look_at(look_at + 0.3f * direction);
        }
    }
private:
    zombye::rendering_system& renderer_;
};

zombye::play_state::play_state(zombye::state_machine *sm) : sm_(sm) {
    auto input = sm->get_game()->input();
    input_ = input->create_manager();

    input_->register_command("FIRE", new test_command());
    input_->register_command("CAMFOWARD", new cam_forward{*sm->get_game()});
    input_->register_command("CAMBACKWARD", new cam_backward{*sm->get_game()});
    input_->register_command("CAMLEFT", new cam_left{*sm->get_game()});
    input_->register_command("CAMRIGHT", new cam_right{*sm->get_game()});
    input_->register_command("switch_state", new switch_anim{*sm->get_game()});

    auto first_joystick = input->first_joystick();

    if(first_joystick) {
        input_->register_event("FIRE", first_joystick->button_A());
        input_->register_event("FIRE", first_joystick->button_B());
    }

    input_->register_event("FIRE", input->mouse()->left_button());
    input_->register_keyboard_event("FIRE", "space");
    input_->register_keyboard_event("CAMFOWARD", "w");
    input_->register_keyboard_event("CAMBACKWARD", "s");
    input_->register_keyboard_event("CAMLEFT", "a");
    input_->register_keyboard_event("CAMRIGHT", "d");
    input_->register_keyboard_event("switch_state", "x");
}

void zombye::play_state::enter() {
    zombye::log("enter play state");

    auto& camera = sm_->get_game()->entity_manager().emplace(glm::vec3{5.f, 2.f, 0.f}, glm::angleAxis(0.f, glm::vec3{0.f, 0.f, 0.f}), glm::vec3{1.f});
    camera.emplace<camera_component>(glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f});
    sm_->get_game()->rendering_system().activate_camera(camera.id());

    auto& ani = sm_->get_game()->entity_manager().emplace("qdummy", glm::vec3{0.f}, glm::angleAxis(0.f, glm::vec3{0.f, 0.f, 0.f}), glm::vec3{1.f});
    ani.component<zombye::animation_component>()->change_state("move");

    sm_->get_game()->entity_manager().emplace("light", glm::vec3{5.f, 20.f, 10.f}, glm::quat{0.f, 0.f, 1.f, 0.f}, glm::vec3{1.f});
}

void zombye::play_state::leave() {
    zombye::log("leave play state");
}

void zombye::play_state::update(float delta_time) {
    auto command = input_->handle_input();

    if(command != nullptr) {
        command->execute();
    }
}

#include <cmath>

#include <zombye/gameplay/states/play_state.hpp>

class test_command : public zombye::command {
public:
    test_command() {}

    void execute() {
        zombye::log("FIRE!!!! PENG PENG!!!");
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
            auto direction = glm::normalize(look_at - pos);
            owner.position(pos + 0.3f * direction);
            cam->set_look_at(look_at + 0.3f * direction);
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
            auto direction = glm::normalize(look_at - pos);
            owner.position(pos - 0.3f * direction);
            cam->set_look_at(look_at - 0.3f * direction);
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
            auto direction = glm::normalize(look_at - pos);
            auto up = cam->up();
            direction = glm::normalize(glm::cross(direction, up));
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
            auto direction = glm::normalize(look_at - pos);
            auto up = cam->up();
            direction = glm::normalize(glm::cross(direction, up));
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
}

void zombye::play_state::enter() {
    zombye::log("enter play state");

    auto& camera = sm_->get_game()->entity_manager().emplace(glm::vec3{0.f, 0.f, 4.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    camera.emplace<camera_component>(glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f});
    sm_->get_game()->rendering_system().activate_camera(camera.id());
/*
    sm_->get_game()->entity_manager().emplace("light_test", glm::vec3{0.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("light", glm::vec3{0.f, 2.f, -6.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("red_light", glm::vec3{4.f, 1.f, -5.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("green_light", glm::vec3{4.f, 0.f, -8.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("red_light", glm::vec3{4.f, 0.f, 8.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("animated", glm::vec3{4.f, 1.f, -6.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
*/
    sm_->get_game()->entity_manager().emplace("building", glm::vec3{0.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
    sm_->get_game()->entity_manager().emplace("light", glm::vec3{10.f, 50.f, 10.f}, glm::normalize(glm::quat{}), glm::vec3{1.f});
}

void zombye::play_state::leave() {
    zombye::log("leave play state");
}

void zombye::play_state::update(float delta_time) {
    auto command = input_->handle_input();

    if(command != nullptr) {
        command->execute();
    }

    auto cam = sm_->get_game()->rendering_system().active_camera();
    if (cam) {
        auto input = sm_->get_game()->input();
        auto x = (input->mouse()->x() / sm_->get_game()->width()) - 1.f;
        auto y = (input->mouse()->y() / sm_->get_game()->height()) - 1.f;
        x *= M_PI;
        y *= M_PI;
        auto pos = cam->owner().position();
        auto look_at = cam->look_at();
        auto up = cam->up();
        auto r = glm::length(look_at - pos);
        auto sx = r * cosf(y) * cosf(x);
        auto sy = r * sinf(y);
        auto sz = r * cosf(y) * sinf(x);
        cam->set_look_at(glm::vec3{sx, sy, sz});
    }
}

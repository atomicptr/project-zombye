#include <zombye/audio/audio_system.hpp>
#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_loader.hpp>
#include <zombye/assets/asset_manager.hpp>
#include <zombye/assets/native_loader.hpp>
#include <zombye/config/config_system.hpp>
#include <zombye/ecs/entity_manager.hpp>
#include <zombye/ecs/rtti_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/gameplay/camera_follow_component.hpp>
#include <zombye/gameplay/gameplay_system.hpp>
#include <zombye/gameplay/game_states.hpp>
#include <zombye/gameplay/state_component.hpp>
#include <zombye/input/input_manager.hpp>
#include <zombye/input/input_system.hpp>
#include <zombye/input/keyboard.hpp>
#include <zombye/input/mouse.hpp>
#include <zombye/physics/character_physics_component.hpp>
#include <zombye/physics/physics_component.hpp>
#include <zombye/physics/physics_system.hpp>
#include <zombye/physics/shapes/box_shape.hpp>
#include <zombye/physics/shapes/triangle_mesh_shape.hpp>
#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/animation_system.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/directional_light_component.hpp>
#include <zombye/rendering/no_occluder_component.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/shadow_component.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/scripting/scripting_system.hpp>
#include <zombye/utils/fps_counter.hpp>
#include <zombye/utils/sdlhelper.hpp>
#include <zombye/utils/state_machine.hpp>
#include <zombye/utils/logger.hpp>
#include <zombye/utils/os.h>

zombye::game::game(std::string title) : title_(title), running_(false), window_(nullptr, SDL_DestroyWindow) {
#ifdef ZOMBYE_DEBUG
    log("DEBUG BUILD");
#endif

    log("init game with OS: " + std::string(OS_NAME));

    asset_manager_ = std::unique_ptr<zombye::asset_manager>{new zombye::asset_manager{}};
    config_system_ = std::unique_ptr<zombye::config_system>(new zombye::config_system());

    width_ = config_system_->get("main", "width").asInt();
    height_ = config_system_->get("main", "height").asInt();
    fullscreen_ = config_system_->get("main", "fullscreen").asBool();

    SDL_Init(SDL_INIT_EVERYTHING);

    scripting_system_ = std::make_unique<zombye::scripting_system>(*this);
    entity_manager_ = std::unique_ptr<zombye::entity_manager>(new zombye::entity_manager(*this));
    register_components();

    auto mask = SDL_WINDOW_OPENGL | (fullscreen_ ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    window_ = make_window(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_,
        height_, mask);

    SDL_GetWindowSize(window_.get(), &width_, &height_);

    auto error = std::string{ SDL_GetError() };

    if (!window_) {
        throw std::runtime_error("could not create window: " + error);
    }

    SDL_ClearError();

    input_system_ = std::unique_ptr<zombye::input_system>(new zombye::input_system(config()));
    audio_system_ = std::unique_ptr<zombye::audio_system>(new zombye::audio_system());
    rendering_system_ = std::unique_ptr<zombye::rendering_system>(new zombye::rendering_system(*this, window_.get()));
    animation_system_ = std::make_unique<zombye::animation_system>(*this);
    physics_system_ = std::unique_ptr<zombye::physics_system>(new zombye::physics_system(*this));
    gameplay_system_ = std::unique_ptr<zombye::gameplay_system>(new zombye::gameplay_system(this));
}

zombye::game::~game() {
    log("quit game");

    SDL_Quit();
}

void zombye::game::run() {
    running_ = true;

    SDL_Event event;

    float delta_time;
    float current_time = 0.f;
    float old_time;

    // start menu state
    gameplay_system_->use(GAME_STATE_MENU);

    auto fps = fps_counter{};

    while(running_) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit();
            }

            if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                width_ = event.window.data1;
                height_ = event.window.data2;

                log("resized window to { width: " + std::to_string(width_) + ", height: " +
                    std::to_string(height_) + " }");
            }

            // handle input
            input_system_->update(event);
        }

        old_time = current_time;
        current_time = SDL_GetTicks() / 1000.f;
        delta_time = current_time - old_time;

        input_system_->update_continuous();

        physics_system_->update(delta_time);
        gameplay_system_->update(delta_time);
        animation_system_->update(delta_time);

        rendering_system_->begin_scene();
        rendering_system_->update(delta_time);
        physics_system_->debug_draw();
        rendering_system_->end_scene();
        SDL_GL_SwapWindow(window_.get());

        entity_manager_->clear();

#ifdef ZOMBYE_DEBUG
        update_fps(delta_time);
#endif
    }

    // to ensure that the last game state "leaves" before SDL_Quit
    gameplay_system_->dispose_current();
}

void zombye::game::quit() {
    running_ = false;
}

void zombye::game::register_components() {
    rtti_manager::register_type(animation_component::type_rtti());
    rtti_manager::register_type(light_component::type_rtti());
    rtti_manager::register_type(staticmesh_component::type_rtti());

    animation_component::register_at_script_engine(*this);
    camera_component::register_at_script_engine(*this);
    camera_follow_component::register_at_script_engine(*this);
    character_physics_component::register_at_script_engine(*this);
    light_component::register_at_script_engine(*this);
    directional_light_component::register_at_script_engine(*this);
    physics_component::register_at_script_engine(*this);
    box_shape::register_at_script_engine(*this);
    shadow_component::register_at_script_engine(*this);
    state_component::register_at_script_engine(*this);
    staticmesh_component::register_at_script_engine(*this);
    triangle_mesh_shape::register_at_script_engine(*this);
    no_occluder_component::register_at_script_engine(*this);
}

int zombye::game::width() const {
    return width_;
}

int zombye::game::height() const {
    return height_;
}

zombye::input_system* zombye::game::input() {
    return input_system_.get();
}

zombye::audio_system* zombye::game::audio() {
    return audio_system_.get();
}

zombye::gameplay_system* zombye::game::gameplay() {
    return gameplay_system_.get();
}

zombye::config_system* zombye::game::config() {
    return config_system_.get();
}

zombye::physics_system* zombye::game::physics() {
    return physics_system_.get();
}

void zombye::game::update_fps(float delta_time) {
    const auto smooth_factor = 0.1f;
    static auto delta_time_smoothed = 0.f;
    static auto time_since_last_fps_output = 0.f;

    delta_time_smoothed = ( 1.0f - smooth_factor) * delta_time_smoothed + smooth_factor * delta_time;

    time_since_last_fps_output += delta_time;
    if(time_since_last_fps_output >= 1.0f){
        time_since_last_fps_output = 0.0f;
        std::ostringstream osstr;
        osstr << title_ << " (" << (int((1.0f / delta_time_smoothed) * 10.0f) / 10.0f) << " FPS, ";
        osstr << (int(delta_time_smoothed * 10000.0f) / 10.0f) << " ms / frame)";
        SDL_SetWindowTitle(window_.get(), osstr.str().c_str());
    }
}

int glCreateGame(const char* name) {
    zombye::game game{name};

    game.run();

    return 0;
}

#include <zombye/core/game.hpp>
#include <zombye/ecs/rtti_manager.hpp>
#include <zombye/ecs/health_component.hpp>
#include <zombye/ecs/staticmesh_component.hpp>

zombye::game::game(std::string title, int width, int height) :
    title_(title), width_(width), height_(height), running_(false) {

    zombye::log("init game with OS: " + std::string(OS_NAME));

    SDL_Init(SDL_INIT_EVERYTHING);

    rtti_manager::register_type(health_component::type_rtti());
    rtti_manager::register_type(staticmesh_component::type_rtti());

    entity_manager_ = std::unique_ptr<zombye::entity_manager>(new zombye::entity_manager(*this));
    input_system_ = std::unique_ptr<zombye::input_system>(new zombye::input_system());
    audio_system_ = std::unique_ptr<zombye::audio_system>(new zombye::audio_system());
}

zombye::game::~game() {
    zombye::log("quit game");

    SDL_Quit();
}

void zombye::game::update(float delta_time) {
    // TODO: wibbly wobbly timey wimey stuff
    static auto& ent = entity_manager_->emplace(glm::vec3(), glm::quat(), glm::vec3());
    ent.emplace("staticmesh_component", "models/space_goblin.msh");
    ent.emplace("health_component", 10.f, 20.f);
}

void zombye::game::run() {
    running_ = true;

    // create window
    auto mask = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    auto window = zombye::make_window(title_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width_, height_, mask);

    SDL_Event event;

    float delta_time;
    float current_time = 0.f;
    float old_time;

    while(running_) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit();
            }

            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                width_ = event.window.data1;
                height_ = event.window.data2;

                zombye::log("resized window to { width: " + std::to_string(width_) + ", height: " +
                    std::to_string(height_) + " }");
            }

            // handle input
            input_system_->update(event);
        }

        old_time = current_time;
        current_time = SDL_GetTicks() / 1000.f;
        delta_time = current_time - old_time;

        update(delta_time);

        entity_manager_->clear();
    }
}

void zombye::game::quit() {
    running_ = false;
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

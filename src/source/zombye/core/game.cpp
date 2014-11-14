#include <zombye/core/game.hpp>
#include <zombye/ecs/rtti_manager.hpp>

zombye::game::game(std::string title) : title_(title), running_(false) {
    zombye::log("init game with OS: " + std::string(OS_NAME));

    config_system_ = std::unique_ptr<zombye::config_system>(new zombye::config_system());

    width_ = config_system_->get("main", "width").asInt();
    height_ = config_system_->get("main", "height").asInt();
    fullscreen_ = config_system_->get("main", "fullscreen").asBool();

    SDL_Init(SDL_INIT_EVERYTHING);

    register_components();

    input_system_ = std::unique_ptr<zombye::input_system>(new zombye::input_system(config()));
    audio_system_ = std::unique_ptr<zombye::audio_system>(new zombye::audio_system());
    entity_manager_ = std::unique_ptr<zombye::entity_manager>(new zombye::entity_manager(*this));
    gameplay_system_ = std::unique_ptr<zombye::gameplay_system>(new zombye::gameplay_system(this));
}

zombye::game::~game() {
    zombye::log("quit game");

    SDL_Quit();
}

void zombye::game::run() {
    running_ = true;

    // create window
    auto mask = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    if(fullscreen_) {
        mask = mask | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    auto window = zombye::make_window(title_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width_, height_, mask);

    SDL_Event event;

    float delta_time;
    float current_time = 0.f;
    float old_time;

    // start menu state
    gameplay_system_->use(GAME_STATE_MENU);

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

        gameplay_system_->update(delta_time);
        entity_manager_->clear();
    }

    // to ensure that the last game state "leaves" before SDL_Quit
    gameplay_system_->dispose_current();
}

void zombye::game::quit() {
    running_ = false;
}

void zombye::game::register_components() {

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

int glCreateGame(const char* name) {
    zombye::game game{name};

    game.run();

    return 0;
}

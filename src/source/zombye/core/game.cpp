#include <zombye/core/game.hpp>

zombye::game::game(std::string title, int width, int height) :
    title_(title), width_(width), height_(height), running_(false) {
}

zombye::game::~game() {

}

void zombye::game::run() {
    running_ = true;

    SDL_Init(SDL_INIT_EVERYTHING);

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
                running_ = false;
            }

            // TODO: handle input
        }

        old_time = current_time;
        current_time = SDL_GetTicks() / 1000.f;
        delta_time = current_time - old_time;
    }

    SDL_Quit();
}

// TODO: window is resizable so width and height can't stay fixed
int zombye::game::width() const {
    return width_;
}

int zombye::game::height() const {
    return height_;
}

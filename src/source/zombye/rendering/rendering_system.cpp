#include <string>

#include <zombye/core/game.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    rendering_system::rendering_system(game& game, SDL_Window* window)
    : game_{game}, window_{window} {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        context_ = SDL_GL_CreateContext(window_);
        auto error = std::string{SDL_GetError()};
        if (error != "") {
            log(LOG_FATAL, "could not create OpenGL context with version 3.1: " + error);
        }
        SDL_ClearError();

        SDL_GL_SetSwapInterval(1);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            log(LOG_FATAL, "could not initialize glew");
        }

        auto version = std::string{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
        log("OpenGL version " + version);

        glEnable(GL_DEPTH_TEST);
        clear_color(0.4, 0.5, 0.9, 1.0);
    }

    rendering_system::~rendering_system() {
        window_ = nullptr;
        SDL_GL_DeleteContext(context_);
    }

    void rendering_system::begin_scene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void rendering_system::end_scene() {
        SDL_GL_SwapWindow(window_);
    }

    void rendering_system::update(float delta_time) {

    }

    void rendering_system::clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }
}
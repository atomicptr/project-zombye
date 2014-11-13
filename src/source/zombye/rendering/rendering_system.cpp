#include <string>

#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/shader_program.hpp>
#include <zombye/rendering/vertex_buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_layout.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    static std::shared_ptr<const shader> vs;
    static std::shared_ptr<const shader> fs;
    static std::unique_ptr<shader_program> sp;
    static std::unique_ptr<vertex_buffer> vb;
    static std::unique_ptr<vertex_array> va;
    static std::unique_ptr<vertex_layout> vl;

    struct vertex {
        glm::vec3 pos;
        glm::vec4 col;
    };

    rendering_system::rendering_system(game& game, SDL_Window* window)
    : game_(game), window_(window), shader_manager_() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        context_ = SDL_GL_CreateContext(window);
        auto error = std::string{SDL_GetError()};
        if (error == "\0") {
            throw std::runtime_error("could not create OpenGL context with version 3.3: " + error);
        }
        SDL_ClearError();

        SDL_GL_SetSwapInterval(1);

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("could not initialize glew");
        }

        auto version = glGetString(GL_VERSION);
        log("OpenGL version " + std::string{reinterpret_cast<const char*>(version)});

        glEnable(GL_DEPTH_TEST);
        set_clear_color(0.5f, 0.5f, 0.5f, 1.0f);

        if (GLEW_KHR_debug) {
            glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar *message, void *userParam) {
                log(LOG_ERROR, std::string(message, length) + " source: " + std::to_string(source)
                    + " type: " +  std::to_string(type) + " id: " + std::to_string(id)
                    + " servity: " + std::to_string(severity));
            }, nullptr);
        } else {
            log(LOG_ERROR, "no OpenGL debug log available");
        }

        vs = shader_manager_.load("shader/color.vs", GL_VERTEX_SHADER);
        fs = shader_manager_.load("shader/color.fs", GL_FRAGMENT_SHADER);
        sp = std::unique_ptr<shader_program>{new shader_program{}};
        vb = std::unique_ptr<vertex_buffer>{new vertex_buffer{0, GL_STATIC_DRAW}};
        va = std::unique_ptr<vertex_array>{new vertex_array{}};
        vl = std::unique_ptr<vertex_layout>{new vertex_layout{}};
        vl->emplace("in_position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        vl->emplace("in_color", 4, GL_FLOAT, GL_FALSE, sizeof(vertex), sizeof(glm::vec3));
        sp->attach_shader(vs);
        sp->attach_shader(fs);
        vl->setup_layout(*va, &vb);
        vl->setup_program(*sp, "fragcolor");
        sp->link();

        vertex vertex[3] = {
            {glm::vec3{0.f, 0.5f, 0.f}, glm::vec4{1.f, 0.f, 0.f, 1.f}},
            {glm::vec3{0.5f, -0.5f, 0.f}, glm::vec4{0.f, 1.f, 0.f, 1.f}},
            {glm::vec3{-0.5f, -0.5f, 0.f}, glm::vec4{0.f, 0.f, 1.f, 1.f}}
        };

        vb->data(3 * sizeof(vertex), vertex);
    }

    rendering_system::~rendering_system() noexcept {
        window_ = nullptr;
        SDL_GL_DeleteContext(context_);
    }

    void rendering_system::update(float delta_time) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: fancy rendering
        va->bind();
        sp->use();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

        SDL_GL_SwapWindow(window_);
    }

    void rendering_system::set_clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }
}
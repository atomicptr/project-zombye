#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        glm::vec3 vertices[4];
        vertices[0] = glm::vec3{-0.5f, 0.5f, 0.0f};
        vertices[1] = glm::vec3{-0.5f, -0.5f, 0.0f};
        vertices[2] = glm::vec3{0.5f, -0.5f, 0.0f};
        vertices[3] = glm::vec3{0.5f, 0.5f, 0.0f};
        quad_ = std::make_unique<vertex_buffer>(4 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

        unsigned int indices[6];
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;
        ibo_ = std::make_unique<index_buffer>(6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        std::string vs_source = "#version 140\nin vec3 position;\nuniform mat4 mvp;\nvoid main() {\ngl_Position = mvp * vec4(position, 1.0);\n}";
        vertex_shader_ = std::make_shared<const shader>("simple_vs", GL_VERTEX_SHADER, vs_source);

        std::string fs_source = "#version 140\nout vec4 fragcolor;\nvoid main() {\nfragcolor = vec4(1.0, 0.0, 0.0, 1.0);\n}";
        fragment_shader_ = std::make_shared<const shader>("simple_fs", GL_FRAGMENT_SHADER, fs_source);

        program_ = std::make_unique<program>();
        program_->attach_shader(vertex_shader_);
        program_->attach_shader(fragment_shader_);

        vao_ = std::make_unique<vertex_array>();
        layout_.emplace_back("position", 3, GL_FLOAT, GL_FALSE, 0, 0);
        layout_.setup_layout(*vao_, &quad_);
        layout_.setup_program(*program_, "fragcolor");
        vao_->bind_index_buffer(*ibo_);

        program_->link();

        float fovy = 90.f * 3.1415f / 180.f;
        float aspect = static_cast<float>(game_.width()) / static_cast<float>(game_.height());
        float near = 0.01f;
        float far = 1000.f;
        projection_ = glm::perspective(fovy, aspect, near, far);
        view_ = glm::lookAt(glm::vec3{2.f, 2.f, 3.f}, glm::vec3{0.f}, glm::vec3{0.f, 1.f, 0.f});
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
        program_->use();
        vao_->bind();
        program_->uniform("mvp", false, projection_ * view_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void rendering_system::clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }
}
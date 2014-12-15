#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <zombye/core/game.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/utils/logger.hpp>
#include <zombye/utils/load_dds.hpp>

namespace zombye {
    struct vertex {
        glm::vec3 pos;
        glm::vec2 tex;
    };

    rendering_system::rendering_system(game& game, SDL_Window* window)
    : game_{game}, window_{window}, shader_manager_{game_} {
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

        vertex vertices[4];
        vertices[0].pos = glm::vec3{-0.5f, 0.5f, 0.0f};
        vertices[0].tex = glm::vec2{0.f, 0.f};
        vertices[1].pos = glm::vec3{-0.5f, -0.5f, 0.0f};
        vertices[1].tex = glm::vec2{0.f, 1.f};
        vertices[2].pos = glm::vec3{0.5f, -0.5f, 0.0f};
        vertices[2].tex = glm::vec2{1.f, 1.f};
        vertices[3].pos = glm::vec3{0.5f, 0.5f, 0.0f};
        vertices[3].tex = glm::vec2{1.f, 0.f};
        quad_ = std::make_unique<vertex_buffer>(4 * sizeof(vertex), vertices, GL_STATIC_DRAW);

        unsigned int indices[6];
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;
        ibo_ = std::make_unique<index_buffer>(6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        vertex_shader_ = shader_manager_.load("shader/staticmesh.vs", GL_VERTEX_SHADER);
        fragment_shader_ = shader_manager_.load("shader/staticmesh.fs", GL_FRAGMENT_SHADER);

        program_ = std::make_unique<program>();
        program_->attach_shader(vertex_shader_);
        program_->attach_shader(fragment_shader_);

        vao_ = std::make_unique<vertex_array>();
        layout_.emplace_back("position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        layout_.emplace_back("texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vertex), sizeof(glm::vec3));
        layout_.setup_layout(*vao_, &quad_);
        layout_.setup_program(*program_, "fragcolor");
        vao_->bind_index_buffer(*ibo_);

        program_->link();

        auto asset = game_.asset_manager().load("texture/dummy.dds");
        texture_ = std::make_shared<const texture>(gli::texture2D{gli::load_dds(asset->content())});

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
        program_->uniform("diffuse", 0);
        texture_->bind(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void rendering_system::clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }
}
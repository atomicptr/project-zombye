#include <string>

#include <glm/gtc/matrix_transform.hpp>

#include <zombye/assets/asset_manager.hpp>
#include <zombye/core/game.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/shader_program.hpp>
#include <zombye/rendering/vertex_buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_layout.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    rendering_system::rendering_system(game& game, SDL_Window* window)
    : game_(game), window_(window), mesh_manager_{*this} {
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

        vertex_layout_.emplace("in_position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        vertex_layout_.emplace("in_normals", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), sizeof(glm::vec3));
        vertex_layout_.emplace("in_texel", 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 2 * sizeof(glm::vec3));

        auto vs = shader_manager_.load("shader/staticmesh.vs", GL_VERTEX_SHADER);
        if (!vs) {
            throw std::runtime_error("could not load shader from file shader/staticmesh.vs");
        }
        auto fs = shader_manager_.load("shader/staticmesh.fs", GL_FRAGMENT_SHADER);
        if (!fs) {
            throw std::runtime_error("could not load shader from file shader/staticmesh.fs");
        }

        staticmesh_program_ = std::unique_ptr<shader_program>{new shader_program{}};

        staticmesh_program_->attach_shader(vs);
        staticmesh_program_->attach_shader(fs);

        vertex_layout_.setup_program(*staticmesh_program_, "fragcolor");
        staticmesh_program_->link();
    }

    rendering_system::~rendering_system() noexcept {
        window_ = nullptr;
        SDL_GL_DeleteContext(context_);
    }

    void rendering_system::update(float delta_time) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game_.entity_manager().emplace("dummy", glm::vec3{}, glm::quat{}, glm::vec3{});
        // TODO: fancy rendering
        auto p = glm::perspective(90.0f * 3.14f / 180.0f, 800.0f / 600.0f, 0.01f, 1000.0f);
        auto v = glm::lookAt(glm::vec3{-2.f, 2.f, -3.f}, glm::vec3{0.f}, glm::vec3{0.f, 1.f, 0.f});
        auto vp = p * v;
        staticmesh_program_->use();
        staticmesh_program_->uniform("vp", 1, GL_FALSE, vp);
        staticmesh_program_->uniform("diffuse", 0);
        for (auto& sm : staticmesh_components_) {
            sm->diffuse_texture()->bind(0);
            sm->mesh()->draw();
        }

        SDL_GL_SwapWindow(window_);
    }

    void rendering_system::set_clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void rendering_system::register_component(staticmesh_component* component) {
        staticmesh_components_.emplace_back(component);
    }

    void rendering_system::unregister_component(staticmesh_component* component) {
        auto it = std::find(staticmesh_components_.begin(), staticmesh_components_.end() ,component);
        auto last = staticmesh_components_.end() - 1;
        if (it != last) {
            *it = std::move(*last);
        }
        staticmesh_components_.pop_back();
    }
}
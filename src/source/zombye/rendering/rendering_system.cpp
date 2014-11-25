#include <algorithm>
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
#include <zombye/utils/os.h>

namespace zombye {
    rendering_system::rendering_system(game& game, SDL_Window* window)
    : game_(game), window_(window), mesh_manager_{game_}, texture_manager_{game_},
    shader_manager_{game_}, active_camera_{0}, perspective_projection_{1} {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        context_ = SDL_GL_CreateContext(window);
        auto error = std::string{SDL_GetError()};
        if (error != "\0") {
            throw std::runtime_error("could not create OpenGL context with version 3.3: " + error);
        }
        SDL_ClearError();

        SDL_GL_SetSwapInterval(1);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("could not initialize glew");
        }

        auto version = glGetString(GL_VERSION);
        log("OpenGL version " + std::string{reinterpret_cast<const char*>(version)});

        glEnable(GL_DEPTH_TEST);
        set_clear_color(0.5f, 0.5f, 0.5f, 1.0f);

        if (GLEW_KHR_debug) {
#ifdef __APPLE__
            glDebugMessageCallback(+[](GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar *message, const void *userParam) {
                log(LOG_ERROR, std::string(message, length) + " source: " + std::to_string(source)
                    + " type: " +  std::to_string(type) + " id: " + std::to_string(id)
                    + " servity: " + std::to_string(severity));
            }, nullptr);
#else
            glDebugMessageCallback(+[](GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar *message, void *userParam) {
                log(LOG_ERROR, std::string(message, length) + " source: " + std::to_string(source)
                    + " type: " +  std::to_string(type) + " id: " + std::to_string(id)
                    + " servity: " + std::to_string(severity));
            }, nullptr);
#endif
        } else {
            log(LOG_ERROR, "no OpenGL debug log available");
        }

        vertex_layout_.emplace("in_position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
        vertex_layout_.emplace("in_normal", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), sizeof(glm::vec3));
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

        fovy_ = 90.0f * 3.14f / 180.0f;
        auto aspect_ratio = static_cast<float>(game_.width()) / static_cast<float>(game_.height());
        near_plane_ = 0.01f;
        far_plane_ = 1000.0f;
        perspective_projection_ = glm::perspective(fovy_, aspect_ratio, near_plane_, far_plane_);
    }

    rendering_system::~rendering_system() noexcept {
        window_ = nullptr;
        SDL_GL_DeleteContext(context_);
    }

    void rendering_system::update(float delta_time) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = glm::mat4{1.0f};
        auto camera = camera_components_.find(active_camera_);
        if (camera != camera_components_.end()) {
            view = camera->second->transform();
        }
        auto vp = perspective_projection_ * view;

        staticmesh_program_->use();
        staticmesh_program_->uniform("color_texture", 0);
        for (auto& sm : staticmesh_components_) {
            staticmesh_program_->uniform("mvp", 1, GL_FALSE, vp * sm->owner().transform());
            sm->draw();
        }

        SDL_GL_SwapWindow(window_);
    }

    void rendering_system::set_clear_color(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void rendering_system::resize_projection(float width, float height) noexcept {
        auto aspect_ratio = width / height;
        perspective_projection_ = glm::perspective(fovy_, aspect_ratio, near_plane_, far_plane_);
        glViewport(0, 0, width, height);
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

    void rendering_system::register_component(camera_component* component) {
        camera_components_.emplace(std::make_pair(component->owner().id(), component));
    }

    void rendering_system::unregister_component(camera_component* component) {
        auto it = camera_components_.find(component->owner().id());
        if (it != camera_components_.end()) {
            camera_components_.erase(it);
        }
    }
}
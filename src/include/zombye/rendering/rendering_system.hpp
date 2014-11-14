#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/mesh_manager.hpp>
#include <zombye/rendering/shader_manager.hpp>
#include <zombye/rendering/shader_program.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/texture_manager.hpp>
#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    class game;
    class rendering_system {
        friend class staticmesh_component;
        friend class camera_component;

        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;
        mesh_manager mesh_manager_;
        shader_manager shader_manager_;
        texture_manager texture_manager_;
        vertex_layout vertex_layout_;
        std::unique_ptr<shader_program> staticmesh_program_;
        std::vector<staticmesh_component*> staticmesh_components_;
        std::unordered_map<unsigned long, camera_component*> camera_components_;
        unsigned long active_camera_;
        glm::mat4 perspective_projection_;
        float fovy_;
        float near_plane_;
        float far_plane_;

        void register_component(staticmesh_component* component);
        void unregister_component(staticmesh_component* component);
        void register_component(camera_component* component);
        void unregister_component(camera_component* component);
    public:
        rendering_system(game& game, SDL_Window* window);
        rendering_system(const rendering_system& other) = delete;
        rendering_system(rendering_system&& other) = delete;
        ~rendering_system() noexcept;

        void update(float delta_time);

        void set_clear_color(float red, float green, float blue, float alpha);

        auto& get_mesh_manager() {
            return mesh_manager_;
        }

        auto& get_shader_manager() {
            return shader_manager_;
        }

        auto& get_texture_manager() {
            return texture_manager_;
        }

        auto& get_vertex_layout() {
            return vertex_layout_;
        }

        void activate_camera(unsigned long owner_id) {
            if (camera_components_.find(owner_id) != camera_components_.end()) {
                active_camera_ = owner_id;
            }
        }

        unsigned long active_camera() {
            return active_camera_;
        }

        void resize_projection(float width, float height) noexcept;

        rendering_system& operator=(const rendering_system& other) = delete;
        rendering_system& operator=(rendering_system&& other) = delete;
    };
}

#endif
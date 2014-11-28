#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/animation_manager.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/mesh_manager.hpp>
#include <zombye/rendering/rigged_mesh_manager.hpp>
#include <zombye/rendering/shader_manager.hpp>
#include <zombye/rendering/shader_program.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/texture_manager.hpp>
#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    class game;
    class rendering_system {
        friend class animation_component;
        friend class staticmesh_component;
        friend class camera_component;
        friend class light_component;

        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;
        animation_manager animation_manager_;
        mesh_manager mesh_manager_;
        rigged_mesh_manager rigged_mesh_manager_;
        shader_manager shader_manager_;
        texture_manager texture_manager_;
        vertex_layout vertex_layout_;
        vertex_layout skinned_vertex_layout_;
        std::unique_ptr<shader_program> staticmesh_program_;
        std::unique_ptr<shader_program> riggedmesh_program_;
        std::vector<animation_component*> animation_components_;
        std::vector<light_component*> light_components_;
        std::vector<staticmesh_component*> staticmesh_components_;
        std::unordered_map<unsigned long, camera_component*> camera_components_;
        unsigned long active_camera_;
        glm::mat4 perspective_projection_;
        float fovy_;
        float near_plane_;
        float far_plane_;

        void register_component(animation_component* component);
        void unregister_component(animation_component* component);
        void register_component(staticmesh_component* component);
        void unregister_component(staticmesh_component* component);
        void register_component(camera_component* component);
        void unregister_component(camera_component* component);
        void register_component(light_component* component);
        void unregister_component(light_component* component);
    public:
        rendering_system(game& game, SDL_Window* window);
        rendering_system(const rendering_system& other) = delete;
        rendering_system(rendering_system&& other) = delete;
        ~rendering_system() noexcept;

        void begin_scene();

        void end_scene();

        void update(float delta_time);

        void set_clear_color(float red, float green, float blue, float alpha);

        auto& get_animation_manager() {
            return animation_manager_;
        }

        auto& get_mesh_manager() {
            return mesh_manager_;
        }

        auto& get_rigged_mesh_manager() {
            return rigged_mesh_manager_;
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

        auto& get_skinned_vertex_layout() {
            return skinned_vertex_layout_;
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

        auto& perspective_projection() {
            return perspective_projection_;
        }

        auto view() {
            auto view = glm::mat4{1.0f};
            auto camera = camera_components_.find(active_camera_);
            if (camera != camera_components_.end()) {
                view = camera->second->transform();
            }
            return view;
        }

        rendering_system& operator=(const rendering_system& other) = delete;
        rendering_system& operator=(rendering_system&& other) = delete;
    };
}

#endif
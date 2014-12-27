#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <memory>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <zombye/rendering/animation_component.hpp>
#include <zombye/rendering/buffer.hpp>
#include <zombye/rendering/camera_component.hpp>
#include <zombye/rendering/light_component.hpp>
#include <zombye/rendering/mesh_manager.hpp>
#include <zombye/rendering/program.hpp>
#include <zombye/rendering/shader.hpp>
#include <zombye/rendering/shader_manager.hpp>
#include <zombye/rendering/skeleton_manager.hpp>
#include <zombye/rendering/skinned_mesh_manager.hpp>
#include <zombye/rendering/staticmesh_component.hpp>
#include <zombye/rendering/texture.hpp>
#include <zombye/rendering/texture_manager.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    class rendering_system {
        friend class animation_component;
        friend class camera_component;
        friend class light_component;
        friend class staticmesh_component;

        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;

        std::vector<animation_component*> animation_components_;
        std::unordered_map<unsigned long, camera_component*> camera_components_;
        std::vector<light_component*> light_components_;
        std::vector<staticmesh_component*> staticmesh_components_;

        std::unique_ptr<program> animation_program_;
        std::unique_ptr<program> staticmesh_program_;
        vertex_layout skinnedmesh_layout_;
        vertex_layout staticmesh_layout_;
        zombye::mesh_manager mesh_manager_;
        zombye::texture_manager texture_manager_;
        zombye::shader_manager shader_manager_;
        zombye::skinned_mesh_manager skinned_mesh_manager_;
        zombye::skeleton_manager skeleton_manager_;
        unsigned long active_camera_;

        glm::mat4 projection_;

    public:
        rendering_system(game& game, SDL_Window* window);
        rendering_system(const rendering_system& other) = delete;
        rendering_system(rendering_system&& other) = delete;
        ~rendering_system();
        rendering_system& operator=(const rendering_system& other) = delete;
        rendering_system& operator=(rendering_system&& other) = delete;

        void begin_scene();
        void end_scene();
        void update(float delta_time);
        void clear_color(float red, float green, float blue, float alpha);

        void activate_camera(unsigned long owner_id) {
            if (camera_components_.find(owner_id) != camera_components_.end()) {
                active_camera_ = owner_id;
            }
        }

        unsigned long active_camera() {
            return active_camera_;
        }

        auto& mesh_manager() noexcept {
            return mesh_manager_;
        }

        auto& shader_manager() noexcept {
            return shader_manager_;
        }

        auto& skinnedmesh_layout() noexcept {
            return skinnedmesh_layout_;
        }

        auto& skinned_mesh_manager() noexcept {
            return skinned_mesh_manager_;
        }

        auto& skeleton_manager() noexcept {
            return skeleton_manager_;
        }

        auto& staticmesh_layout() noexcept {
            return staticmesh_layout_;
        }

        auto& texture_manager() noexcept {
            return texture_manager_;
        }

    private:
        void register_component(animation_component* component);
        void unregister_component(animation_component* component);
        void register_component(camera_component* component);
        void unregister_component(camera_component* component);
        void register_component(light_component* component);
        void unregister_component(light_component* component);
        void register_component(staticmesh_component* component);
        void unregister_component(staticmesh_component* component);
    };
}

#endif
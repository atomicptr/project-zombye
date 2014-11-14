#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>

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

        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;
        mesh_manager mesh_manager_;
        shader_manager shader_manager_;
        texture_manager texture_manager_;
        vertex_layout vertex_layout_;
        std::unique_ptr<shader_program> staticmesh_program_;
        std::vector<staticmesh_component*> staticmesh_components_;

        void register_component(staticmesh_component* component);
        void unregister_component(staticmesh_component* component);
    public:
        rendering_system(game& game, SDL_Window* window);
        rendering_system(const rendering_system& other) = delete;
        rendering_system(rendering_system&& other) = delete;
        ~rendering_system() noexcept;

        void update(float delta_time);

        void set_clear_color(float red, float green, float blue, float alpha);

        auto& mesh_manager() {
            return mesh_manager_;
        }

        auto& shader_manager() {
            return shader_manager_;
        }

        auto& texture_manager() {
            return texture_manager_;
        }

        auto& vertex_layout() {
            return vertex_layout_;
        }

        rendering_system& operator=(const rendering_system& other) = delete;
        rendering_system& operator=(rendering_system&& other) = delete;
    };
}

#endif
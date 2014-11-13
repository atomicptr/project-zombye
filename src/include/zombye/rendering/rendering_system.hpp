#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <zombye/rendering/shader_manager.hpp>
#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    class game;
    class rendering_system {
        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;
        shader_manager shader_manager_;
        zombye::vertex_layout vertex_layout_;
    public:
        rendering_system(game& game, SDL_Window* window);
        rendering_system(const rendering_system& other) = delete;
        rendering_system(rendering_system&& other) = delete;
        ~rendering_system() noexcept;

        void update(float delta_time);

        void set_clear_color(float red, float green, float blue, float alpha);

        zombye::vertex_layout& vertex_layout() {
            return vertex_layout_;
        }

        rendering_system& operator=(const rendering_system& other) = delete;
        rendering_system& operator=(rendering_system&& other) = delete;
    };
}

#endif
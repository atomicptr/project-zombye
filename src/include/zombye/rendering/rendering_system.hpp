#ifndef __ZOMBYE_RENDERING_SYSTEM_HPP__
#define __ZOMBYE_RENDERING_SYSTEM_HPP__

#include <memory>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <zombye/rendering/buffer.hpp>

namespace zombye {
    class game;
}

namespace zombye {
    class rendering_system {
        game& game_;
        SDL_Window* window_;
        SDL_GLContext context_;

        std::unique_ptr<vertex_buffer> quad_;
        std::unique_ptr<index_buffer> ibo_;

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
    };
}

#endif
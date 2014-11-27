#ifndef __ZOMBYE_DEBUG_RENDERER_HPP__
#define __ZOMBYE_DEBUG_RENDERER_HPP__

#include <memory>

#include <glm/glm.hpp>

#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_buffer.hpp>
#include <zombye/rendering/vertex_layout.hpp>
#include <zombye/rendering/shader.hpp>
#include <zombye/rendering/shader_program.hpp>

namespace zombye {
    struct debug_vertex {
        glm::vec3 pos;
        glm::vec3 col;
    };

    class game;
    class rendering_system;
    class debug_renderer {
        game& game_;
        rendering_system& rs_;
        vertex_layout debug_vertex_layout_;
        std::unique_ptr<vertex_buffer> vbo_;
        vertex_array vao_;
        shader_program physics_debug_program_;

    public:
        debug_renderer(game& game);

        void draw_line(const glm::vec3&, const glm::vec3&, const glm::vec3&);
        void draw_contact_point(const glm::vec3&, const glm::vec3&, float, int, const glm::vec3&);
        void draw_transform(glm::vec3&, glm::quat&, float);

    };
}

#endif

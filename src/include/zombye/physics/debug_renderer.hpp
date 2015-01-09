#ifndef __ZOMBYE_DEBUG_RENDERER_HPP__
#define __ZOMBYE_DEBUG_RENDERER_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/buffer.hpp>
#include <zombye/rendering/vertex_layout.hpp>
#include <zombye/rendering/shader.hpp>
#include <zombye/rendering/program.hpp>

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
        vertex_buffer vbo_;
        vertex_array vao_;
        program debug_program_;
        std::vector<debug_vertex> line_buffer_;
        std::vector<debug_vertex> point_buffer_;

    public:
        debug_renderer(game& game);

        void buffer_line(const glm::vec3&, const glm::vec3&, const glm::vec3&);
        void buffer_contact_point(const glm::vec3& point, const glm::vec3& normal, float distance, float lifetime, const glm::vec3& color);

        void draw();
    };
}

#endif

#include <stdexcept>

#include <zombye/core/game.hpp>
#include <zombye/physics/debug_renderer.hpp>
#include <zombye/rendering/rendering_system.hpp>

namespace zombye {
    debug_renderer::debug_renderer(game& game)
    : game_{game}, rs_{game_.rendering_system()} {
        debug_vertex_layout_.emplace("in_position", 3, GL_FLOAT, GL_FALSE, sizeof(debug_vertex), 0);
        debug_vertex_layout_.emplace("in_color", 3, GL_FLOAT, GL_FALSE, sizeof(debug_vertex), sizeof(glm::vec3));

        auto& shader_manager_ =rs_.get_shader_manager();

        auto vs = shader_manager_.load("shader/physics_debug.vs", GL_VERTEX_SHADER);
        if (!vs) {
            throw std::runtime_error("could not load shader from file physics_debug.vs");
        }

        auto fs = shader_manager_.load("shader/physics_debug.fs", GL_FRAGMENT_SHADER);
        if (!fs) {
            throw std::runtime_error("could not load shader from file physics_debug.fs");
        }

        vbo_ = std::make_unique<vertex_buffer>(2 * sizeof(debug_vertex), GL_DYNAMIC_DRAW);

        debug_vertex_layout_.setup_layout(vao_, &vbo_);

        physics_debug_program_.attach_shader(vs);
        physics_debug_program_.attach_shader(fs);

        debug_vertex_layout_.setup_program(physics_debug_program_, "fragcolor");
        physics_debug_program_.link();

        glPointSize(10);
        glLineWidth(5);
    }

    void debug_renderer::draw_line(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) {
        debug_vertex v[2];
        v[0].pos = from;
        v[0].col = color;
        v[1].pos = to;
        v[1].col = color;
        vbo_->data(2 * sizeof(debug_vertex), v);
        physics_debug_program_.use();
        physics_debug_program_.uniform("vp", 1, GL_FALSE, rs_.perspective_projection() * rs_.view());
        vao_.bind();
        glDrawArrays(GL_LINES, 0, 2);
    }

    void debug_renderer::draw_contact_point(const glm::vec3& point, const glm::vec3& normal, float distance,
        int lifetime, const glm::vec3& color) {
        debug_vertex v;
        v.pos = point;
        v.col = color;
        vbo_->data(sizeof(debug_vertex), &v);
        physics_debug_program_.use();
        physics_debug_program_.uniform("vp", 1, GL_FALSE, rs_.perspective_projection() * rs_.view());
        vao_.bind();
        glDrawArrays(GL_POINTS, 0, 1);
    }
}
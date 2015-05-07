#include <zombye/core/game.hpp>
#include <zombye/physics/debug_renderer.hpp>
#include <zombye/rendering/rendering_system.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    debug_renderer::debug_renderer(game& game)
    : game_{game}, rs_{game_.rendering_system()}, vbo_{0, GL_DYNAMIC_DRAW} {
        debug_vertex_layout_.emplace_back("position", 3, GL_FLOAT, GL_FALSE, sizeof(debug_vertex), 0);
        debug_vertex_layout_.emplace_back("color", 3, GL_FLOAT, GL_FALSE, sizeof(debug_vertex), sizeof(glm::vec3));

        auto& shader_manager_ =rs_.shader_manager();

        auto vs = shader_manager_.load("shader/physics_debug.vs", GL_VERTEX_SHADER);
        if (!vs) {
            log(LOG_FATAL, "could not load shader from file physics_debug.vs");
        }

        auto fs = shader_manager_.load("shader/physics_debug.fs", GL_FRAGMENT_SHADER);
        if (!fs) {
            log(LOG_FATAL, "could not load shader from file physics_debug.fs");
        }

        debug_vertex_layout_.setup_layout(vao_, &vbo_);

        debug_program_.attach_shader(vs);
        debug_program_.attach_shader(fs);

        debug_vertex_layout_.setup_program(debug_program_, "fragcolor");
        debug_program_.link();
    }

    void debug_renderer::buffer_line(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) {
        debug_vertex v[2];
        v[0].pos = from;
        v[0].col = color;
        v[1].pos = to;
        v[1].col = color;
        line_buffer_.emplace_back(v[0]);
        line_buffer_.emplace_back(v[1]);
    }

    void debug_renderer::buffer_contact_point(const glm::vec3& point, const glm::vec3& normal, float distance, float lifetime, const glm::vec3& color) {
        point_buffer_.emplace_back(debug_vertex{point, color});
    }


    void debug_renderer::draw() {
        vbo_.data(line_buffer_.size() * sizeof(debug_vertex), line_buffer_.data());
        vao_.bind();
        debug_program_.use();
        debug_program_.uniform("vp", GL_FALSE, rs_.projection() * rs_.view());
        glDrawArrays(GL_LINES, 0, line_buffer_.size());
        line_buffer_.clear();

        vbo_.data(point_buffer_.size() * sizeof(debug_vertex), point_buffer_.data());
        vao_.bind();
        debug_program_.uniform("vp", GL_FALSE, rs_.projection() * rs_.view());
        glDrawArrays(GL_POINTS, 0, point_buffer_.size());
        point_buffer_.clear();
    }
}

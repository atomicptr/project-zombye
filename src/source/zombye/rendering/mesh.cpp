#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    mesh::mesh(rendering_system& rendering_system, const std::vector<char> data)
    : vertex_count_{*reinterpret_cast<const size_t*>(data.data())},
    vbo_{new vertex_buffer{vertex_count_ * sizeof(vertex),
    reinterpret_cast<const vertex*>(data.data() + sizeof(size_t)), GL_STATIC_DRAW}} {
        rendering_system.vertex_layout().setup_layout(vao_, &vbo_);
    }

    void mesh::draw() const {
        vao_.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
    }
}
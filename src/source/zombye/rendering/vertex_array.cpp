#include <zombye/rendering/vertex_array.hpp>
#include <iostream>

namespace zombye {
    vertex_array::vertex_array() noexcept
    : name_{0} {
        glGenVertexArrays(1, &name_);
    }

    vertex_array::~vertex_array() noexcept {
        glDeleteVertexArrays(1, &name_);
    }

    void vertex_array::bind_vertex_attribute(const vertex_buffer& vbo, uint32_t index, int32_t size,
        GLenum type, bool normalized, size_t stride, intptr_t offset) noexcept {
        glBindVertexArray(name_);
        vbo.bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride,
            reinterpret_cast<GLvoid*>(offset));
    }

    void vertex_array::bind_index_buffer(const index_buffer& ibo) noexcept {
        glBindVertexArray(name_);
        ibo.bind();
    }

    void vertex_array::bind() const noexcept {
        glBindVertexArray(name_);
    }
}
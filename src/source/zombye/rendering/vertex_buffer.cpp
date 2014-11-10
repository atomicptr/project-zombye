#include <zombye/rendering/vertex_buffer.hpp>

namespace zombye {
    vertex_buffer::vertex_buffer(size_t size, GLenum usage) noexcept
    : buffer{GL_ARRAY_BUFFER, size, usage} { }

    vertex_buffer::vertex_buffer(size_t size, const void* data, GLenum usage) noexcept
    : buffer{GL_ARRAY_BUFFER, size, data, usage} { }
}
#include <zombye/rendering/index_buffer.hpp>

namespace zombye {
    index_buffer::index_buffer(size_t size, GLenum usage) noexcept
    : buffer{GL_ELEMENT_ARRAY_BUFFER, size, usage} { }

    index_buffer::index_buffer(size_t size, const void* data, GLenum usage) noexcept
    : buffer{GL_ELEMENT_ARRAY_BUFFER, size, data, usage} { }
}
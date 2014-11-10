#ifndef __ZOMBYE_VERTEX_BUFFER_HPP__
#define __ZOMBYE_VERTEX_BUFFER_HPP__

#include <zombye/rendering/buffer.hpp>

namespace zombye {
    class vertex_buffer : public buffer {
    public:
        vertex_buffer(size_t size, GLenum usage) noexcept;
        vertex_buffer(size_t size, const void* data, GLenum usage) noexcept;
        ~vertex_buffer() noexcept = default;
    };
}

#endif
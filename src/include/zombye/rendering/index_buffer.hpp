#ifndef __ZOMBYE_INDEX_BUFFER_HPP__
#define __ZOMBYE_INDEX_BUFFER_HPP__

#include <zombye/rendering/buffer.hpp>

namespace zombye {
    class index_buffer : public buffer {
    public:
        index_buffer(size_t size, GLenum usage) noexcept;
        index_buffer(size_t size, const void* data, GLenum usage) noexcept;
        ~index_buffer() noexcept = default;
    };
}

#endif
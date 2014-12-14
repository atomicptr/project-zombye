#ifndef __ZOMBYE_VERTEX_ARRAY_HPP__
#define __ZOMBYE_VERTEX_ARRAY_HPP__

#include <GL/glew.h>

#include <zombye/rendering/buffer.hpp>

namespace zombye {
    class vertex_array {
        friend class vertex_layout;
        friend class rendering_system;

        GLuint id_;
    public:
        vertex_array() noexcept;
        vertex_array(const vertex_array& other) = delete;
        vertex_array(vertex_array&& other) = default;
        ~vertex_array() noexcept;
        vertex_array& operator=(const vertex_array& other) = delete;
        vertex_array& operator=(vertex_array&& other) = default;

        void bind() const noexcept;
    private:
        void bind_vertex_attribute(const vertex_buffer& buffer, uint32_t index, int32_t size, GLenum type,
        bool normalized, size_t stride, intptr_t offset) noexcept;
        void bind_index_buffer(const index_buffer& buffer);
    };
}

#endif
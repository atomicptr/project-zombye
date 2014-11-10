#ifndef __ZOMBYE_VERTEX_ARRAY_HPP__
#define __ZOMBYE_VERTEX_ARRAY_HPP__

#include <zombye/rendering/vertex_buffer.hpp>
#include <zombye/rendering/index_buffer.hpp>

namespace zombye {
    class vertex_array {
        GLuint name_;
    public:
        vertex_array() noexcept;
        vertex_array(const vertex_array& other) = delete;
        vertex_array(vertex_array&& other) = delete;
        ~vertex_array() noexcept;

        void bind_vertex_attribute(const vertex_buffer& vbo, uint32_t index, int32_t size, GLenum type,
            bool normalized, size_t stride, intptr_t offset) noexcept;
        void bind_index_buffer(const index_buffer& ibo) noexcept;
        void bind() const noexcept;


        vertex_array& operator=(const vertex_array& other) = delete;
        vertex_array& operator=(vertex_array& other) = delete;
    };
}

#endif
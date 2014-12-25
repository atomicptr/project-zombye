#ifndef __ZOMBYE_BUFFER_HPP__
#define __ZOMBYE_BUFFER_HPP__

#include <GL/glew.h>

namespace zombye {
    template <GLenum target>
    class buffer {
        GLuint id_;
        GLenum usage_;
    public:
        buffer(size_t size, GLenum usage) noexcept
        : usage_{usage} {
            glGenBuffers(1, &id_);
            glBindBuffer(target, id_);
            glBufferData(target, size, nullptr, usage_);
        }

        buffer(size_t size, const void* data, GLenum usage) noexcept
        : usage_{usage} {
            glGenBuffers(1, &id_);
            glBindBuffer(target, id_);
            glBufferData(target, size, data, usage_);
        }

        buffer(const buffer& other) = delete;

        buffer(buffer&& other) noexcept
        : id_{other.id_}, usage_{other.usage_} {
            id_ = 0;
        }

        ~buffer() noexcept {
            glDeleteBuffers(1, &id_);
        }

        buffer& operator=(const buffer& other) = delete;

        buffer& operator=(buffer&& other) noexcept {
            id_ = other.id_;
            usage_ = other.usage_;
            other.id_ = 0;

            return *this;
        }

        void data(size_t size, const void* data) noexcept {
            glBindBuffer(target, id_);
            glBufferData(target, size, data, usage_);
        }

        void subdata(intptr_t offset, size_t size, const void* data) noexcept {
            glBindBuffer(target, id_);
            glBufferSubData(target, offset, size, data);
        }

        void bind() const noexcept {
            glBindBuffer(target, id_);
        }
    };

    using vertex_buffer = buffer<GL_ARRAY_BUFFER>;
    using index_buffer = buffer<GL_ELEMENT_ARRAY_BUFFER>;
}

#endif
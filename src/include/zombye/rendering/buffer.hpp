#ifndef __ZOMBYE_BUFFER_HPP__
#define __ZOMBYE_BUFFER_HPP__

#include <GL/glew.h>

namespace zombye {
    class buffer {
        GLuint name_;
        GLenum target_;
        GLenum usage_;
    public:
        buffer(GLuint target, size_t size, GLenum usage) noexcept;
        buffer(GLuint target, size_t size, const void* data, GLenum usage) noexcept;
        buffer(const buffer& other) = delete;
        buffer(buffer&& other) = delete;
        virtual ~buffer() noexcept = 0;

        void data(size_t size, const void* data) noexcept;
        void sub_data(intptr_t offset, size_t size, const void* data) noexcept;
        void bind() const noexcept;

        buffer& operator=(const buffer& other) = delete;
        buffer& operator=(buffer&& other) = delete;
    };
}

#endif
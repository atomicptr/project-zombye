#include <zombye/rendering/buffer.hpp>

namespace zombye {
    buffer::buffer(GLuint target, size_t size, GLenum usage) noexcept
    : name_{0}, target_{target}, usage_{usage} {
        glGenBuffers(1, &name_);
        glBindBuffer(target_, name_);
        glBufferData(target_, size, nullptr, usage_);
    }

    buffer::buffer(GLuint target, size_t size, const void* data, GLenum usage) noexcept
    : name_{0}, target_{target}, usage_{usage} {
        glGenBuffers(1, &name_);
        glBindBuffer(target_, name_);
        glBufferData(target_, size, data, usage_);
    }

    buffer::~buffer() noexcept {
        glDeleteBuffers(1, &name_);
    }

    void buffer::data(size_t size, const void* data) noexcept {
        glBindBuffer(target_, name_);
        glBufferData(target_, size, data, usage_);
    }

    void buffer::sub_data(intptr_t offset, size_t size, const void* data) noexcept {
        glBindBuffer(target_, name_);
        glBufferSubData(target_, offset, size, data);
    }

    void buffer::bind() const noexcept {
        glBindBuffer(target_, name_);
    }
}
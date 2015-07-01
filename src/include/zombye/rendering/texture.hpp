#ifndef __ZOMBYE_TEXTURE_HPP__
#define __ZOMBYE_TEXTURE_HPP__

#include <GL/glew.h>
#include <gli/gli.hpp>

namespace zombye {
    class texture {
    private:
        GLuint id_;
        GLenum target_;
        size_t width_;
        size_t height_;

    public:
        texture(const gli::texture2D& texture) noexcept;
        texture(GLenum target, GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data = nullptr) noexcept;
        texture(GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data = nullptr) noexcept;
        ~texture();

        texture(const texture& rhs) = delete;
        texture& operator=(const texture& rhs) = delete;

        texture(texture&& rhs) noexcept;
        texture& operator=(texture&& rhs) noexcept;

        void bind(uint32_t unit) const noexcept;

        size_t width() const noexcept {
            return width_;
        }

        size_t height() const noexcept {
            return height_;
        }

        void apply_settings() const noexcept;

    private:
        friend class framebuffer;
    };
}

#endif

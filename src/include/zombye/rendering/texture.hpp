#ifndef __ZOMBYE_TEXTURE_HPP__
#define __ZOMBYE_TEXTURE_HPP__

#include <GL/glew.h>
#include <gli/gli.hpp>

namespace zombye {
    class texture {
        GLuint id_;
        size_t width_;
        size_t height_;
    public:
        texture(const gli::texture2D& texture) noexcept;
        texture(const texture& other) = delete;
        texture(texture&& other) noexcept = default;
        ~texture() noexcept;
        texture& operator=(const texture& other) = delete;
        texture& operator=(texture&& other) noexcept = default;

        void bind(uint32_t unit) const noexcept;

        size_t width() {
            return width_;
        }

        size_t height() {
            return height_;
        }
    };
}

#endif
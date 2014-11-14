#ifndef __ZOMBYE_TEXTURE_HPP__
#define __ZOMBYE_TEXTURE_HPP__

#include <GL/glew.h>
#include <gli/gli.hpp>

namespace zombye {
    class texture {
        GLuint name_;
        size_t width_;
        size_t height_;
    public:
        texture(const gli::texture2D tex_image) noexcept;
        texture(const texture& other) = delete;
        texture(texture&& other) = delete;
        ~texture() noexcept;

        void bind(uint32_t unit) const noexcept;

        texture& operator=(const texture& other) = delete;
        texture& operator=(texture&& other) = delete;
    };
}

#endif
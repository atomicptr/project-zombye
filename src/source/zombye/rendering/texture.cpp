#include <zombye/rendering/texture.hpp>

namespace zombye {
    texture::texture(const gli::texture2D& tex_image) noexcept {
        glGenTextures(1, &name_);
        glBindTexture(GL_TEXTURE_2D, name_);
        // adapted from http://www.g-truc.net/project-0024.html
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(tex_image.levels() - 1));
        if (gli::is_compressed(tex_image.format())) {
            for (gli::texture2D::size_type level = 0; level < tex_image.levels(); ++level) {
                glCompressedTexImage2D(GL_TEXTURE_2D,
                    GLint(level),
                    GLenum{gli::internal_format(tex_image.format())},
                    GLsizei(tex_image[level].dimensions().x),
                    GLsizei(tex_image[level].dimensions().y),
                    0,
                    GLsizei(tex_image[level].size()),
                    tex_image[level].data());
            }
        } else {
            for (gli::texture2D::size_type level = 0; level < tex_image.levels(); ++level) {
                glTexImage2D(GL_TEXTURE_2D,
                    GLint(level),
                    GLenum(gli::internal_format(tex_image.format())),
                    GLsizei(tex_image[level].dimensions().x),
                    GLsizei(tex_image[level].dimensions().y),
                    0,
                    GLenum(gli::external_format(tex_image.format())),
                    GLenum(gli::type_format(tex_image.format())),
                    tex_image[level].data());
            }
        }
    }

    texture::~texture() noexcept {
        glDeleteTextures(1, &name_);
    }

    void texture::bind(uint32_t unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, name_);
    }
}
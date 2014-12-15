#include <zombye/rendering/texture.hpp>

namespace zombye {
    texture::texture(const gli::texture2D& texture) noexcept
    : width_{texture[0].dimensions().x}, height_{texture[0].dimensions().y} {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        // adapted from http://www.g-truc.net/project-0024.html
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(texture.levels() - 1));
        if (gli::is_compressed(texture.format())) {
            for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
                glCompressedTexImage2D(GL_TEXTURE_2D,
                GLint(level),
                GLenum{gli::internal_format(texture.format())},
                GLsizei(texture[level].dimensions().x),
                GLsizei(texture[level].dimensions().y),
                0,
                GLsizei(texture[level].size()),
                texture[level].data());
            }
        } else {
            for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
                glTexImage2D(GL_TEXTURE_2D,
                GLint(level),
                GLenum(gli::internal_format(texture.format())),
                GLsizei(texture[level].dimensions().x),
                GLsizei(texture[level].dimensions().y),
                0,
                GLenum(gli::external_format(texture.format())),
                GLenum(gli::type_format(texture.format())),
                texture[level].data());
            }
        }
    }

    texture::~texture() {
        glDeleteTextures(1, &id_);
    }

    void texture::bind(uint32_t unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }
}
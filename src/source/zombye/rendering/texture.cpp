#include <zombye/rendering/texture.hpp>

namespace zombye {
    texture::texture(const gli::texture2D& texture) noexcept
    : width_{texture.dimensions().x}, height_{texture.dimensions().y} {
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    texture::texture(texture&& other) noexcept
    : id_{other.id_}, width_{other.width_}, height_{other.height_} {
        other.id_ = 0;
        other.width_ = 0;
        other.height_ = 0;
    }

    texture::~texture() noexcept {
        glDeleteTextures(1, &id_);
    }

    texture& texture::operator=(texture&& other) noexcept {
        id_ = other.id_;
        width_ = other.width_;
        height_ = other.height_;
        other.id_ = 0;
        other.width_ = 0;
        other.height_ = 0;

        return *this;
    }

    void texture::bind(uint32_t unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }
}
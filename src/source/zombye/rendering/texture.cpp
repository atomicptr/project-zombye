#include <zombye/rendering/texture.hpp>

namespace zombye {
    texture::texture(const gli::texture2D& texture) noexcept
    : width_{texture.dimensions().x}, height_{texture.dimensions().y}, target_{GL_TEXTURE_2D} {
        glGenTextures(1, &id_);
        glBindTexture(target_, id_);
        apply_settings();
        // adapted from http://gli.g-truc.net/0.5.1/code.html
        if (gli::is_compressed(texture.format())) {
            for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
                glCompressedTexImage2D(target_,
                static_cast<GLint>(level),
                static_cast<GLenum>(gli::internal_format(texture.format())),
                static_cast<GLsizei>(texture[level].dimensions().x),
                static_cast<GLsizei>(texture[level].dimensions().y),
                0,
                static_cast<GLsizei>(texture[level].size()),
                texture[level].data());
            }
        } else {
            for (gli::texture2D::size_type level = 0; level < texture.levels(); ++level) {
                glTexImage2D(target_,
                static_cast<GLint>(level),
                static_cast<GLenum>(gli::internal_format(texture.format())),
                static_cast<GLsizei>(texture[level].dimensions().x),
                static_cast<GLsizei>(texture[level].dimensions().y),
                0,
                static_cast<GLenum>(gli::external_format(texture.format())),
                static_cast<GLenum>(gli::type_format(texture.format())),
                texture[level].data());
            }
        }
    }

    texture::texture(GLenum target, GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data) noexcept
    : width_{static_cast<size_t>(width)}, height_{static_cast<size_t>(height)}, target_{target} {
        glGenTextures(1, &id_);
        glBindTexture(target_, id_);
        glTexImage2D(target_, 0, internal_format, width_, height_, 0, format, type, data);
        glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    texture::~texture() {
        glDeleteTextures(1, &id_);
    }

    texture::texture(texture&& rhs) noexcept
    : id_{rhs.id_}, target_{rhs.target_}, width_{rhs.width_}, height_{rhs.height_} {
        rhs.id_ = 0;
    }

    texture& texture::operator=(texture&& rhs) noexcept {
        id_ = rhs.id_;
        target_ = rhs.target_;
        width_ = rhs.width_;
        height_ = rhs.height_;
        rhs.id_ = 0;
        return *this;
    }

    void texture::bind(uint32_t unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target_, id_);
    }

    void texture::apply_settings() const noexcept {
        glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        auto max_aniso = 0.f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
        glTexParameteri(target_, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
    }
}

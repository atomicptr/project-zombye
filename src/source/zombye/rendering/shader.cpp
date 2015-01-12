#include <vector>

#include <zombye/rendering/shader.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    shader::shader(const std::string& name, GLenum type, const std::string& source) {
        id_ = glCreateShader(type);

        auto source_ptr = source.c_str();
        glShaderSource(id_, 1, &source_ptr, nullptr);

        glCompileShader(id_);

        auto length = 0;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            auto log_buffer = std::vector<char>(length);
            glGetShaderInfoLog(id_, length, nullptr, log_buffer.data());
            log("compilation log of " + name + ":");
            log(std::string{log_buffer.begin(), log_buffer.end()});
        }

        auto status = 0;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
        if (!status) {
            glDeleteShader(id_);
            log(LOG_FATAL, "an error occured during compilation of " + name);
        }
    }

    shader::shader(shader&& other) noexcept
    : id_{other.id_} {
        other.id_ = 0;
    }

    shader::~shader() noexcept {
        glDeleteShader(id_);
    }

    shader& shader::operator=(shader&& other) noexcept {
        id_ = other.id_;
        other.id_ = 0;

        return *this;
    }
}

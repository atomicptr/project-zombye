#include <stdexcept>
#include <vector>

#include <zombye/rendering/shader.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    shader::shader(const std::string& file_name, GLenum type, const std::string& source)
    : name_{0} {
        glCreateShader(type);
        const char* tmp = source.c_str();
        glShaderSource(name_, 1, &tmp, nullptr);
        glCompileShader(name_);
        int32_t length = 0;
        glGetShaderiv(name_, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log_buffer;
        if (length > 1) {
            log_buffer.resize(length);
            glGetShaderInfoLog(name_, length, nullptr, log_buffer.data());
            log(LOG_ERROR, "compilation log of " + file_name + ": "
                + std::string(log_buffer.begin(), log_buffer.end()));
        }
        int32_t status;
        glGetShaderiv(name_, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            glDeleteShader(name_);
            throw std::runtime_error("an error occured during compilation of " + file_name
                + std::string(log_buffer.begin(), log_buffer.end()));
        }
    }

    shader::~shader() noexcept {
        glDeleteShader(name_);
    }
}
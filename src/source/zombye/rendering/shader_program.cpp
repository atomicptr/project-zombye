#include <glm/gtc/type_ptr.hpp>

#include <zombye/rendering/shader_program.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    shader_program::shader_program() noexcept
    : name_{0} {
        name_ = glCreateProgram();
    }

    shader_program::~shader_program() noexcept {
        for (auto& shader : shaders_) {
            glDetachShader(name_, shader->name_);
        }
        glDeleteProgram(name_);
    }

    void shader_program::attach_shader(std::shared_ptr<const zombye::shader>& shader) {
        shaders_.emplace_back(shader);
        glAttachShader(name_, shader->name_);
    }

    void shader_program::bind_attribute_location(uint32_t index, const std::string& name) noexcept {
        glBindAttribLocation(name_, index, name.c_str());
    }

    void shader_program::bind_frag_data_location(uint32_t color_number, const std::string name) noexcept {
        glBindFragDataLocation(name_, color_number, name.c_str());
    }

    void shader_program::link() noexcept {
        glLinkProgram(name_);
        auto length = GLint{0};
        glGetProgramiv(name_, GL_INFO_LOG_LENGTH, &length);
        auto log_buffer = std::vector<char>{};
        if (length > 1) {
            log_buffer.resize(length);
            glGetProgramInfoLog(name_, length, nullptr, log_buffer.data());
            log(LOG_INFO, "link log of program " + std::to_string(name_) + ": "
                + std::string{log_buffer.begin(), log_buffer.end()});
        }
        auto status = GLint{0};
        glGetProgramiv(name_, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            glDeleteProgram(name_);
            throw std::runtime_error("an error occured during linking of program "
                + std::to_string(name_) + ": " + std::string{log_buffer.begin(), log_buffer.end()});
        }
        /* causing some erros on apple machines if no vao is bound
        glValidateProgram(name_);
        glGetProgramiv(name_, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            log_buffer.resize(length);
            glGetProgramInfoLog(name_, length, nullptr, log_buffer.data());
            log(LOG_INFO, "validaion log of program " + std::to_string(name_) + ": "
                + std::string{log_buffer.begin(), log_buffer.end() - 2});
        }
        glGetProgramiv(name_, GL_VALIDATE_STATUS, &status);
        if (status == GL_FALSE) {
            glDeleteProgram(name_);
            throw std::runtime_error("program " + std::to_string(name_) + " is not valid: "
                + std::string{log_buffer.begin(), log_buffer.end()});
        }
        */
    }

    void shader_program::use() const noexcept {
        glUseProgram(name_);
    }

    void shader_program::uniform(const std::string& name, float value) noexcept {
        glUniform1f(glGetUniformLocation(name_, name.c_str()), value);
    }

    void shader_program::uniform(const std::string& name, int32_t value) noexcept {
        glUniform1i(glGetUniformLocation(name_, name.c_str()), value);
    }

    void shader_program::uniform(const std::string& name, uint32_t value) noexcept {
        glUniform1ui(glGetUniformLocation(name_, name.c_str()), value);
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::vec2& value) noexcept {
        glUniform2fv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::vec3& value) noexcept {
        glUniform3fv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::vec4& value) noexcept {
        glUniform4fv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::ivec2& value) noexcept {
        glUniform2iv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::ivec3& value) noexcept {
        glUniform3iv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, const glm::ivec4& value) noexcept {
        glUniform4iv(glGetUniformLocation(name_, name.c_str()), count, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, bool transpose, const glm::mat2& value) noexcept {
        glUniformMatrix2fv(glGetUniformLocation(name_, name.c_str()), count, transpose, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, bool transpose, const glm::mat3& value) noexcept {
        glUniformMatrix3fv(glGetUniformLocation(name_, name.c_str()), count, transpose, glm::value_ptr(value));
    }

    void shader_program::uniform(const std::string& name, size_t count, bool transpose, const glm::mat4& value) noexcept {
        glUniformMatrix4fv(glGetUniformLocation(name_, name.c_str()), count, transpose, glm::value_ptr(value));
    }
}
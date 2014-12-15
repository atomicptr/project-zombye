#include <vector>

#include <zombye/rendering/program.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    program::program() noexcept {
        id_ = glCreateProgram();
    }

    program::~program() noexcept {
        for (auto& shader : shaders_) {
            glDetachShader(id_, shader->id_);
        }
        glDeleteProgram(id_);
    }

    void program::attach_shader(shader_ptr shader) {
        shaders_.emplace_back(shader);
        glAttachShader(id_, shader->id_);
    }

    void program::bind_attribute_location(const std::string& name, uint32_t index) noexcept {
        glBindAttribLocation(id_, index, name.c_str());
    }

    void program::bind_frag_data_location(const std::string& name, uint32_t color_number) noexcept {
        glBindFragDataLocation(id_, color_number, name.c_str());
    }

    void program::link() {
        glLinkProgram(id_);

        auto length = 0;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            auto log_buffer = std::vector<char>(length);
            glGetProgramInfoLog(id_, length, nullptr, log_buffer.data());
            log("link log of program " + std::to_string(id_) + ": ");
            log(std::string{log_buffer.begin(), log_buffer.end()});
        }

        auto status = 0;
        glGetProgramiv(id_, GL_LINK_STATUS, &status);
        if (!status) {
            for (auto& shader : shaders_) {
                glDetachShader(id_, shader->id_);
            }
            glDeleteProgram(id_);
            log(LOG_FATAL, "an error occured during linking of program " + std::to_string(id_));
        }
    }

    void program::use() const noexcept {
        glUseProgram(id_);
    }
}
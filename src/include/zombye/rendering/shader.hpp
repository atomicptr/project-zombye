#ifndef __ZOMBYE_SHADER_HPP__
#define __ZOMBYE_SHADER_HPP__

#include <string>

#include <GL/glew.h>

namespace zombye {
    class shader {
        friend class shader_program;
        GLuint name_;
    public:
        shader(const std::string& file_name, GLenum type, const std::string& source);
        shader(const shader& other) = delete;
        shader(shader&& other) = delete;
        ~shader() noexcept;

        shader& operator=(const shader& other) = delete;
        shader& operator=(shader&& other) = delete;
    };
}

#endif
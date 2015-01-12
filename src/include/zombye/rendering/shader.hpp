#ifndef __ZOMBYE_SHADER_HPP__
#define __ZOMBYE_SHADER_HPP__

#include <string>

#include <GL/glew.h>

namespace zombye {
    class shader {
        friend class program;

        GLuint id_;
    public:
        shader(const std::string& name, GLenum type, const std::string& source);
        shader(const shader& other) = delete;
        shader(shader&& other) noexcept;
        ~shader() noexcept;
        shader& operator=(const shader& other) = delete;
        shader& operator=(shader&& other) noexcept;
    };
}

#endif

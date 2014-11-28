#ifndef __ZOMBYE_SHADER_PROGRAM_HPP__
#define __ZOMBYE_SHADER_PROGRAM_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <zombye/rendering/shader.hpp>

namespace zombye {
    class shader_program {
        GLuint name_;
        std::vector<std::shared_ptr<const shader>> shaders_;
    public:
        shader_program() noexcept;
        shader_program(const shader_program& other) = delete;
        shader_program(shader_program&& other) = delete;
        ~shader_program() noexcept;

        void attach_shader(std::shared_ptr<const zombye::shader>& shader);
        void bind_attribute_location(uint32_t index, const std::string& name) noexcept;
        void bind_frag_data_location(uint32_t color_number, const std::string name) noexcept;
        void link() noexcept;
        void use() const noexcept;

        void uniform(const std::string& name, float value) noexcept;
        void uniform(const std::string& name, int32_t value) noexcept;
        void uniform(const std::string& name, uint32_t value) noexcept;
        void uniform(const std::string& name, const glm::vec2& value) noexcept;
        void uniform(const std::string& name, const glm::vec3& value) noexcept;
        void uniform(const std::string& name, const glm::vec4& value) noexcept;
        void uniform(const std::string& name, const glm::ivec2& value) noexcept;
        void uniform(const std::string& name, const glm::ivec3& value) noexcept;
        void uniform(const std::string& name, const glm::ivec4& value) noexcept;
        void uniform(const std::string& name, bool transpose, const glm::mat2& value) noexcept;
        void uniform(const std::string& name, bool transpose, const glm::mat3& value) noexcept;
        void uniform(const std::string& name, bool transpose, const glm::mat4& value) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<float>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<int32_t>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<uint32_t>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::vec2>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::vec3>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::vec4>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::ivec2>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::ivec3>& values) noexcept;
        void uniform(const std::string& name, size_t count, const std::vector<glm::ivec4>& values) noexcept;
        void uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat2>& values) noexcept;
        void uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat3>& values) noexcept;
        void uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat4>& values) noexcept;

        shader_program& operator=(const shader_program& other) = delete;
        shader_program& operator=(shader_program&& other) = delete;
    };
}

#endif
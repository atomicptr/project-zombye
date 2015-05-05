#ifndef __ZOMBYE_PROGRAM_HPP__
#define __ZOMBYE_PROGRAM_HPP__

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <zombye/rendering/shader.hpp>

namespace zombye {
    using shader_ptr = std::shared_ptr<const shader>;

    class program {
        friend class vertex_layout;

        GLuint id_;
        std::vector<shader_ptr> shaders_;
    public:
        program() noexcept;
        program(const program& other) = delete;
        program(program&& other) noexcept;
        ~program() noexcept;
        program& operator=(const program& other) = delete;
        program& operator=(program&& other) noexcept;

        void attach_shader(shader_ptr shader);
        void link();
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

        void bind_frag_data_location(const std::string& name, uint32_t color_number) noexcept;

    private:
        void bind_attribute_location(const std::string& name, uint32_t index) noexcept;
    };
}

#endif

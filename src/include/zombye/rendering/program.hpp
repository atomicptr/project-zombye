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
        program(program&& other) noexcept = default;
        ~program() noexcept;
        program& operator=(const program& other) = delete;
        program& operator=(program&& other) noexcept = default;

        void attach_shader(shader_ptr shader);
        void link();
        void use() const noexcept;

    private:
        void bind_attribute_location(const std::string& name, uint32_t index) noexcept;
        void bind_frag_data_location(const std::string& name, uint32_t color_number) noexcept;
    };
}

#endif
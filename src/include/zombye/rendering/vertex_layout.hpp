#ifndef __ZOMBYE_VERTEX_LAYOUT_HPP__
#define __ZOMBYE_VERTEX_LAYOUT_HPP__

#include <vector>

#include <zombye/rendering/shader_program.hpp>
#include <zombye/rendering/vertex_buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>

namespace zombye {
    struct vertex_attribute {
        const std::string& name;
        uint32_t size;
        GLenum type;
        bool normalized;
        size_t stride;
        intptr_t offset;
        int index;
    };

    class vertex_layout {
        std::vector<vertex_attribute> vertex_attributes_;
    public:
        vertex_layout() noexcept = default;
        vertex_layout(const vertex_layout& other) = delete;
        vertex_layout(vertex_layout&& other) = delete;

        void emplace(const std::string& name, uint32_t size, GLenum type, bool normalized,
            size_t stride, intptr_t offset, int index = 0);
        void setup_layout(vertex_array& vao, const std::unique_ptr<vertex_buffer>* vbo) noexcept;
        void setup_program(shader_program& program, const std::string& name) noexcept;

        vertex_layout& operator=(const vertex_layout& other) = delete;
        vertex_layout& operator=(vertex_layout&& other) = delete;
    };
}

#endif
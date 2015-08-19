#ifndef __ZOMBYE_VERTEX_LAYOUT_HPP__
#define __ZOMBYE_VERTEX_LAYOUT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <zombye/rendering/buffer.hpp>

namespace zombye {
    class program;
    class vertex_array;
}

namespace zombye {
    struct vertex_attribute {
        std::string name;
        uint32_t size;
        GLenum type;
        bool normalized;
        size_t stride;
        intptr_t offset;
        int index;
        intptr_t component_offset;
        int factor;
        int divisor;
    };

    class vertex_layout {
        std::vector<vertex_attribute> vertex_attributes_;
    public:
        vertex_layout() = default;
        vertex_layout(const vertex_layout& other) = delete;
        vertex_layout(vertex_layout&& other) = delete;
        ~vertex_layout() noexcept = default;
        vertex_layout& operator=(const vertex_layout& other) = delete;
        vertex_layout& operator=(vertex_layout&& other) = delete;

        void emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride,
        intptr_t offset, int index = 0, int divisor = 0);
        void emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride,
        intptr_t offset, intptr_t attribute_size, int factor, int index = 0, int divisor = 0);
        void setup_layout(const vertex_array& vertex_array, const std::unique_ptr<vertex_buffer>* buffers) noexcept;
        void setup_layout(const vertex_array& vertex_array, const vertex_buffer* buffers) noexcept;
        void setup_layout(const vertex_array& vertex_array, const vertex_buffer** buffers) noexcept;
        void setup_program(program& program, const std::string& fragcolor_name) noexcept;
    };
}

#endif

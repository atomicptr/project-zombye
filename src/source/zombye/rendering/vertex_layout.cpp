#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    void vertex_layout::emplace(const std::string& name, uint32_t size, GLenum type, bool normalized,
    size_t stride, intptr_t offset, int index) {
        auto va = vertex_attribute{name, size, type, normalized, stride, offset, index};
        vertex_attributes_.emplace_back(va);
    }

    void vertex_layout::setup_layout(vertex_array& vao, const std::unique_ptr<vertex_buffer>* vbo) noexcept {
        vao.bind();
        auto i = uint32_t{1};
        for (auto& va : vertex_attributes_) {
            vbo[va.index]->bind();
            vao.bind_vertex_attribute(*(vbo[va.index]), i, va.size, va.type, va.normalized, va.stride, va.offset);
            ++i;
        }
    }

    void vertex_layout::setup_program(shader_program& program, const std::string& name) noexcept {
        auto i = uint32_t{1};
        for (auto& va : vertex_attributes_) {
            program.bind_attribute_location(i, va.name);
            ++i;
        }

        program.bind_frag_data_location(0, name);
    }
}
#include <zombye/rendering/program.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    void vertex_layout::emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride,
    intptr_t offset, int index, int divisor) {
        auto attribute = vertex_attribute{name, size, type, normalized, stride, offset, index, 0, 1, divisor};
        vertex_attributes_.emplace_back(attribute);
    }

    void vertex_layout::emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride,
    intptr_t offset, intptr_t attribute_size, int factor, int index, int divisor) {
        auto attribute = vertex_attribute{name, size, type, normalized, stride, intptr_t(offset), index, attribute_size / factor, factor, divisor};
        vertex_attributes_.emplace_back(attribute);
    }

    void vertex_layout::setup_layout(const vertex_array& vertex_array, const std::unique_ptr<vertex_buffer>* buffers) noexcept {
        vertex_array.bind();
        auto i = uint32_t{0};
        for (auto& attribute : vertex_attributes_) {
            for (auto j = 0; j < attribute.factor; ++j) {
                buffers[attribute.index]->bind();
                if (attribute.type == GL_INT) {
                    vertex_array.bind_vertex_attributei(*(buffers[attribute.index]), i, attribute.size, attribute.type,
                        attribute.stride, attribute.offset + j * attribute.component_offset);
                } else {
                    vertex_array.bind_vertex_attribute(*(buffers[attribute.index]), i, attribute.size, attribute.type,
                        attribute.normalized, attribute.stride, attribute.offset + j * attribute.component_offset);
                }
                glVertexAttribDivisor(i, attribute.divisor);
                ++i;
            }
        }
    }

    void vertex_layout::setup_layout(const vertex_array& vertex_array, const vertex_buffer* buffers) noexcept {
        vertex_array.bind();
        auto i = uint32_t{0};
        for (auto& attribute : vertex_attributes_) {
            buffers[attribute.index].bind();
            for (auto j = 0; j < attribute.factor; ++j) {
                if (attribute.type == GL_INT) {
                    vertex_array.bind_vertex_attributei(buffers[attribute.index], i, attribute.size, attribute.type,
                        attribute.stride, attribute.offset + j * attribute.component_offset);
                } else {
                    vertex_array.bind_vertex_attribute(buffers[attribute.index], i, attribute.size, attribute.type,
                        attribute.normalized, attribute.stride, attribute.offset + j * attribute.component_offset);
                }
                glVertexAttribDivisor(i, attribute.divisor);
                ++i;
            }
        }
    }

    void vertex_layout::setup_layout(const vertex_array& vertex_array, const vertex_buffer** buffers) noexcept {
        vertex_array.bind();
        auto i = uint32_t{0};
        for (auto& attribute : vertex_attributes_) {
            buffers[attribute.index]->bind();
            for (auto j = 0; j < attribute.factor; ++j) {
                if (attribute.type == GL_INT) {
                    vertex_array.bind_vertex_attributei(*buffers[attribute.index], i, attribute.size, attribute.type,
                        attribute.stride, attribute.offset + j * attribute.component_offset);
                } else {
                    vertex_array.bind_vertex_attribute(*buffers[attribute.index], i, attribute.size, attribute.type,
                        attribute.normalized, attribute.stride, attribute.offset + j * attribute.component_offset);
                }
                glVertexAttribDivisor(i, attribute.divisor);
                ++i;
            }
        }
    }

    void vertex_layout::setup_program(program& program, const std::string& fragcolor_name) noexcept {
        auto i = uint32_t{0};
        for (auto& attribute : vertex_attributes_) {
            program.bind_attribute_location(attribute.name, i);
            i += attribute.factor;
        }

        program.bind_frag_data_location(fragcolor_name, 0);
    }
}

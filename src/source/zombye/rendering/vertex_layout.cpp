#include <zombye/rendering/vertex_layout.hpp>

namespace zombye {
    void vertex_layout::emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride,
    intptr_t offset, int index) {
        auto attribute = vertex_attribute{name, size, type, normalized, stride, offset, index};
        vertex_attributes_.emplace_back(attribute);
    }

    void vertex_layout::setup_layout(vertex_array& vertex_array, const std::unique_ptr<vertex_buffer>* buffers) noexcept {
        vertex_array.bind();
        auto i = uint32_t{0};
        for (auto& attribute : vertex_attributes_) {
            buffers[attribute.index]->bind();
            vertex_array.bind_vertex_attribute(*(buffers[attribute.index]), i, attribute.size, attribute.type,
                attribute.normalized, attribute.stride, attribute.offset);
            ++i;
        }
    }
}
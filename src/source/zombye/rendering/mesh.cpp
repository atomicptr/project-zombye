#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    mesh::mesh(rendering_system& rendering_system, const std::vector<char>& data) {
        auto data_ptr = data.data();
        vertex_count_ = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        auto size = vertex_count_ * sizeof(vertex);
        vbo_ = std::unique_ptr<vertex_buffer>{new vertex_buffer{size, data_ptr, GL_STATIC_DRAW}};
        data_ptr += size;
        auto submesh_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0; i < submesh_count; ++i) {
            auto submesh = *reinterpret_cast<const zombye::submesh*>(data_ptr);
            submeshes_.emplace_back(submesh);
            data_ptr += sizeof(submesh);
        }
        rendering_system.get_vertex_layout().setup_layout(vao_, &vbo_);
    }

    void mesh::draw(int submesh_index) const {
        glDrawArrays(GL_TRIANGLES, submeshes_[submesh_index].offset, submeshes_[submesh_index].vertex_count);
    }
}
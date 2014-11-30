#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/rigged_mesh.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    rigged_mesh::rigged_mesh(rendering_system& rendering_system, const std::vector<char>& data)
    : ibo_{0, GL_STATIC_DRAW} {
        auto data_ptr = data.data();
        auto bone_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0; i < bone_count; ++i) {
            rigg_.emplace_back(*reinterpret_cast<const bone*>(data_ptr));
            data_ptr += sizeof(bone);
        }

        vertex_count_ = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        auto size = vertex_count_ * sizeof(skinned_vertex);
        vbo_ = std::unique_ptr<vertex_buffer>{new vertex_buffer{size, data_ptr, GL_STATIC_DRAW}};
        data_ptr += size;

        auto index_count_= *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        size = index_count_ * sizeof(unsigned int);
        ibo_.data(size, data_ptr);
        data_ptr += size;

        auto submesh_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0; i < submesh_count; ++i) {
            auto submesh = *reinterpret_cast<const zombye::submesh*>(data_ptr);
            submeshes_.emplace_back(submesh);
            data_ptr += sizeof(submesh);
        }

        vao_.bind_index_buffer(ibo_);
        rendering_system.get_skinned_vertex_layout().setup_layout(vao_, &vbo_);
    }

    void rigged_mesh::draw(int submesh_index) const {
        glDrawElementsBaseVertex(GL_TRIANGLES, submeshes_[submesh_index].index_count, GL_UNSIGNED_INT,
            reinterpret_cast<void*>(submeshes_[submesh_index].offset * sizeof(unsigned int)),
            submeshes_[submesh_index].base_vertex);
    }
}
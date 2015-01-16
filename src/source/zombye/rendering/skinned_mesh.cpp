#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    skinned_mesh::skinned_mesh(rendering_system& rendering_system, const std::vector<char>& source) noexcept
    : vbo_{0, GL_STATIC_DRAW}, ibo_{0, GL_STATIC_DRAW} {
        auto data_ptr = source.data();

        auto vertex_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        auto size = vertex_count * sizeof(skinned_vertex);

        auto ptr = data_ptr;
        for (auto i = 0; i < vertex_count; ++i) {
            auto v = reinterpret_cast<const skinned_vertex*>(ptr);
            log(LOG_DEBUG, "vertice " + std::to_string(i) + ": " + glm::to_string(v->index));
            ptr += sizeof(skinned_vertex);
        }

        vbo_.data(size, data_ptr);
        data_ptr += size;

        auto index_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        size = index_count * sizeof(unsigned int);

        ibo_.data(size, data_ptr);
        data_ptr += size;

        auto submesh_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0u; i < submesh_count; ++i) {
            auto submesh = *reinterpret_cast<const zombye::submesh*>(data_ptr);
            submeshes_.emplace_back(submesh);
            data_ptr += sizeof(submesh);
        }

        vao_.bind_index_buffer(ibo_);
        rendering_system.skinnedmesh_layout().setup_layout(vao_, &vbo_);
    }

    void skinned_mesh::draw(int index) const noexcept {
        glDrawElements(GL_TRIANGLES, submeshes_[index].index_count, GL_UNSIGNED_INT,
            reinterpret_cast<void*>(submeshes_[index].offset * sizeof(unsigned int)));
    }
}
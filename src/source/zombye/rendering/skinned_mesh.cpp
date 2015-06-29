#include <mesh_converter/mesh_converter.hpp>

#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/skinned_mesh.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    skinned_mesh::skinned_mesh(rendering_system& rendering_system, const std::vector<char>& source, const std::string& file_name) noexcept
    : vbo_{0, GL_STATIC_DRAW}, ibo_{0, GL_STATIC_DRAW} {
        auto data_ptr = source.data();

        auto head = *reinterpret_cast<const header*>(data_ptr);
        if (head.magic != 0x31424D5A) {
            throw std::runtime_error(file_name + " is not an zombye mesh file");
        }
        auto vertex_size = head.vertex_count * sizeof(skinned_vertex);
        auto index_size = head.index_count * sizeof(uint32_t);
        auto size = sizeof(header)
            + vertex_size
            + index_size
            + head.submesh_count * sizeof(devtools::submesh);

        if (size != source.size()) {
            throw std::runtime_error(file_name + " has not the apropriate size. expected size: "
                + std::to_string(source.size()) + " calculated size: " + std::to_string(size));
        }
        data_ptr += sizeof(header);

        vbo_.data(vertex_size, data_ptr);
        data_ptr += vertex_size;

        ibo_.data(index_size, data_ptr);
        data_ptr += index_size;

        for (auto i = 0; i < head.submesh_count; ++i) {
            submesh s;

            s.index_count = *reinterpret_cast<const uint64_t*>(data_ptr);
            data_ptr += sizeof(uint64_t);
            s.offset = *reinterpret_cast<const uint64_t*>(data_ptr);
            data_ptr += sizeof(uint64_t);

            auto texture_id = *reinterpret_cast<const uint64_t*>(data_ptr);
            auto texture_name = std::to_string(texture_id) + ".dds";
            s.diffuse = rendering_system.texture_manager().load("texture/" + texture_name);
            if (!s.diffuse) {
                throw std::runtime_error("could not load texure " + texture_name);
            }
            data_ptr += sizeof(uint64_t);

            texture_id = *reinterpret_cast<const uint64_t*>(data_ptr);
            texture_name = std::to_string(texture_id) + ".dds";
            s.normal = rendering_system.texture_manager().load("texture/" + texture_name);
            if (!s.normal) {
                throw std::runtime_error("could not load texure " + texture_name);
            }
            data_ptr += sizeof(uint64_t);

            texture_id = *reinterpret_cast<const uint64_t*>(data_ptr);
            texture_name = std::to_string(texture_id) + ".dds";
            s.material = rendering_system.texture_manager().load("texture/" + texture_name);
            if (!s.material) {
                throw std::runtime_error("could not load texure " + texture_name);
            }
            data_ptr += sizeof(uint64_t);

            submeshes_.emplace_back(s);
        }

        vao_.bind_index_buffer(ibo_);
        rendering_system.skinnedmesh_layout().setup_layout(vao_, &vbo_);
    }

    void skinned_mesh::draw() const noexcept {
        vao_.bind();
        for (auto& sub : submeshes_) {
            sub.diffuse->bind(0);
            sub.material->bind(1);
            sub.normal->bind(2);
            glDrawElements(GL_TRIANGLES, sub.index_count, GL_UNSIGNED_INT,
                reinterpret_cast<void*>(sub.offset * sizeof(unsigned int)));
        }
    }
}

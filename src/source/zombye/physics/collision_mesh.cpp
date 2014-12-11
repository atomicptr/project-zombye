#include <zombye/physics/collision_mesh.hpp>

namespace zombye {
    collision_mesh::collision_mesh(const std::vector<char>& data) {
        auto data_ptr = data.data();

        auto vertex_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        vertices_.reserve(vertex_count);
        for (auto i = 0u; i < vertex_count; ++i) {
            vertices_.emplace_back(*reinterpret_cast<const vertex*>(data_ptr));
            data_ptr += sizeof(vertex);
        }

        auto index_count= *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        indices_.reserve(index_count);
        for (auto i = 0u; i < index_count; ++i) {
            indices_.emplace_back(*reinterpret_cast<const index*>(data_ptr));
            data_ptr += sizeof(index);
        }
    }
}
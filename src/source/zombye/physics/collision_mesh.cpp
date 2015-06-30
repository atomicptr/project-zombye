#include <zombye/physics/collision_mesh.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

#include <string>

namespace zombye {
	collision_mesh::collision_mesh(physics_system& physics_system, const std::vector<char>& source, const std::string& file_name) noexcept {
		auto data_ptr = source.data();

		auto h = *reinterpret_cast<const collision_header*>(data_ptr);
		data_ptr += sizeof(collision_header);

		auto size = sizeof(collision_header)
			+ h.vertex_count * sizeof(glm::vec3)
			+ h.index_count * sizeof(index);

		if (size != source.size()) {
			throw std::runtime_error(file_name + " has not the apropriate size. expected size: "
                + std::to_string(source.size()) + " calculated size: " + std::to_string(size));
		}

		for (auto i = uint64_t{0}; i < h.vertex_count; ++i) {
			collision_vertex v = {*reinterpret_cast<const glm::vec3*>(data_ptr)};
			vertices_.emplace_back(v);
			data_ptr += sizeof(collision_vertex);
		}

		for (auto i = uint64_t{0}; i < h.index_count; ++i) {
			indices_.emplace_back(*reinterpret_cast<const index*>(data_ptr));
			data_ptr += sizeof(index);
		}
	}
}

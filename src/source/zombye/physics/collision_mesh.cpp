#include <zombye/physics/collision_mesh.hpp>
#include <zombye/utils/logger.hpp>

#include <glm/gtx/string_cast.hpp>

#include <string>

namespace zombye {
	collision_mesh::collision_mesh(physics_system& physics_system, const std::vector<char>& source) noexcept {
		auto data_ptr = source.data();

		auto vertex_count = *reinterpret_cast<const size_t*>(data_ptr);
		data_ptr += sizeof(size_t);

		for (auto i = 0; i < vertex_count; ++i) {
			vertices_.emplace_back(*reinterpret_cast<const glm::vec3*>(data_ptr));
			data_ptr += sizeof(glm::vec3);
		}

		auto index_count = *reinterpret_cast<const size_t*>(data_ptr);
		data_ptr += sizeof(size_t);
		for (auto i = 0; i < index_count; ++i) {
			indices_.emplace_back(*reinterpret_cast<const index*>(data_ptr));
			data_ptr += sizeof(index);
		}
	}
}
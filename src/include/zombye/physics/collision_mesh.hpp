#ifndef __ZOMBYE_COLLISION_MESH_HPP__
#define __ZOMBYE_COLLISION_MESH_HPP__

#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/mesh.hpp>

namespace zombye {
    using index = unsigned int;

    class collision_mesh {
        std::vector<vertex> vertices_;
        std::vector<index> indices_;
    public:
        collision_mesh(const std::vector<char>& data);
        collision_mesh(const collision_mesh& other) = delete;
        collision_mesh(collision_mesh&& other) = delete;
        ~collision_mesh() noexcept = default;

        auto vertices() {
            return vertices_;
        }

        auto indices() {
            return indices_;
        }

        collision_mesh& operator=(const collision_mesh& other) = delete;
        collision_mesh& operator=(collision_mesh&& other) = delete;
    };
}

#endif
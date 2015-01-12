#ifndef __ZOMBYE_COLLISION_MESH_HPP__
#define __ZOMBYE_COLLISION_MESH_HPP__

#include <vector>

#include <glm/glm.hpp>

namespace zombye {
    class physics_system;
}

namespace zombye {
    struct collision_vertex {
        glm::vec3 pos;
        collision_vertex(const glm::vec3& pos) : pos{pos} {}
    };

    class collision_mesh {
    public:
        collision_mesh(physics_system& physics_system, const std::vector<char>& source) noexcept;
        ~collision_mesh() = default;

        collision_mesh(const collision_mesh& rhs) = delete;
        collision_mesh& operator=(const collision_mesh& rhs) = delete;

        collision_mesh(collision_mesh&& rhs) = default;
        collision_mesh& operator=(collision_mesh&& rhs) = default;
        
        auto& vertices() const noexcept {
            return vertices_;
        }

        auto& indices() const noexcept {
            return indices_;
        }

    private:
        using index = unsigned int;
        std::vector<collision_vertex> vertices_;
        std::vector<index> indices_;
    };
}

#endif

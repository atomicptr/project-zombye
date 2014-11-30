#ifndef __ZOMBYE_RIGGED_MESH_HPP__
#define __ZOMBYE_RIGGED_MESH_HPP__

#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/index_buffer.hpp>
#include <zombye/rendering/mesh.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_buffer.hpp>

namespace zombye {
    struct bone {
        unsigned int id;
        glm::mat4 transformation;
    };

    struct link {
        unsigned int bone = 0;
        float weight = 0.f;
    };

    struct skinned_vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
        link links[4];
    };

    class rendering_system;
    class rigged_mesh {
        size_t vertex_count_;
        std::vector<submesh> submeshes_;
        std::vector<bone> rigg_;
        vertex_array vao_;
        std::unique_ptr<vertex_buffer> vbo_;
        index_buffer ibo_;
    public:
        rigged_mesh(rendering_system& rendering_system, const std::vector<char>& data);
        rigged_mesh(const rigged_mesh& other) = delete;
        rigged_mesh(rigged_mesh&& other) = delete;
        ~rigged_mesh() noexcept = default;

        void draw(int submesh_index) const;

        auto begin() const {
            return submeshes_.begin();
        }

        auto end() const {
            return submeshes_.end();
        }

        auto& vao() const {
            return vao_;
        }

        rigged_mesh& operator=(const rigged_mesh& other) = delete;
        rigged_mesh& operator=(rigged_mesh&& other) = delete;
    };
}

#endif
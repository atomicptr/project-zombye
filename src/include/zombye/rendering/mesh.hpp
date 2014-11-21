#ifndef __ZOMBYE_RENDERING_MESH_HPP__
#define __ZOMBYE_RENDERING_MESH_HPP__

#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/index_buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>
#include <zombye/rendering/vertex_buffer.hpp>

namespace zombye {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };
    struct submesh {
        size_t offset;
        size_t index_count;
    };
    class rendering_system;
    class mesh {
        size_t vertex_count_;
        std::vector<submesh> submeshes_;
        std::unique_ptr<vertex_buffer> vbo_;
        index_buffer ibo_;
        vertex_array vao_;
    public:
        mesh(rendering_system& rendering_system, const std::vector<char>& data);
        mesh(const mesh& other) = delete;
        mesh(mesh&& other) = delete;
        ~mesh() noexcept = default;

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

        mesh& operator=(const mesh& other) = delete;
        mesh& operator=(mesh&& other) = delete;
    };
}

#endif
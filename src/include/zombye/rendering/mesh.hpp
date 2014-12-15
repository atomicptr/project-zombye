#ifndef __ZOMBYE_MESH_HPP__
#define __ZOMBYE_MESH_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>

namespace zombye {
    class rendering_system;
}

namespace zombye {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };

    struct submesh {
        size_t index_count;
        size_t offset;
    };

    class mesh {
        std::vector<submesh> submeshes_;
        vertex_array vao_;
        vertex_buffer vbo_;
        index_buffer ibo_;
    public:
        mesh(rendering_system& rendering_system, const std::vector<char>& source) noexcept;
        mesh(const mesh& other) = delete;
        mesh(mesh&& other) = default;
        ~mesh() noexcept = default;
        mesh& operator=(const mesh& other) = delete;
        mesh& operator=(mesh&& other) noexcept = default;

        void draw(int index) const noexcept;

        auto& vao() const noexcept {
            return vao_;
        }
    };
}

#endif
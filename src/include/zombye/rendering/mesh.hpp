#ifndef __ZOMBYE_MESH_HPP__
#define __ZOMBYE_MESH_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/buffer.hpp>
#include <zombye/rendering/texture.hpp>
#include <zombye/rendering/vertex_array.hpp>

namespace zombye {
    class rendering_system;
}

namespace zombye {
    struct header {
        unsigned long magic;
        size_t vertex_count;
        size_t index_count;
        size_t submesh_count;
    };

    struct vertex {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    struct submesh {
        size_t index_count;
        size_t offset;
        std::shared_ptr<const texture> diffuse;
        std::shared_ptr<const texture> normal;
        std::shared_ptr<const texture> material;
    };

    class mesh {
        std::vector<submesh> submeshes_;
        vertex_array vao_;
        vertex_buffer vbo_;
        index_buffer ibo_;
    public:
        mesh(rendering_system& rendering_system, const std::vector<char>& source, const std::string& file_name) noexcept;
        mesh(const mesh& other) = delete;
        mesh(mesh&& other) = default;
        ~mesh() noexcept = default;
        mesh& operator=(const mesh& other) = delete;
        mesh& operator=(mesh&& other) noexcept = default;

        void draw() const noexcept;

        auto& vao() const noexcept {
            return vao_;
        }
    };
}

#endif

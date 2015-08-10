#ifndef __ZOMBYE_MESH_HPP__
#define __ZOMBYE_MESH_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <zombye/rendering/buffer.hpp>
#include <zombye/rendering/vertex_array.hpp>

namespace zombye {
    class rendering_system;
    class texture;
}

namespace zombye {
    struct header {
        uint32_t magic;
        uint64_t vertex_count;
        uint64_t index_count;
        uint64_t submesh_count;
        bool parallax_mapping;
    };

    struct vertex {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    struct submesh {
        uint64_t index_count;
        uint64_t offset;
        std::shared_ptr<const texture> diffuse;
        std::shared_ptr<const texture> normal;
        std::shared_ptr<const texture> material;
    };

    class mesh {
        std::vector<submesh> submeshes_;
        vertex_array vao_;
        vertex_buffer vbo_;
        index_buffer ibo_;
        bool parallax_mapping_;
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

        auto parallax_mapping() const {
            return parallax_mapping_;
        }
    };
}

#endif

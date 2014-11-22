#ifndef __DEVTOOLS_MESH_CONVERTER_HPP__
#define __DEVTOOLS_MESH_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

namespace devtools {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };

    struct submesh {
        size_t index_count;
        size_t offset;
        size_t base_vertex;
    };

    using index = unsigned int;
    using material = std::string;

    class mesh_converter {
        std::vector<submesh> submeshes_;
        std::vector<vertex> vertices_;
        std::vector<index> indices_;
        std::vector<material> materials_;
        Assimp::Importer is_;
        std::ofstream os_;
        const aiScene* scene_;
    public:
        mesh_converter(const std::string& in, const std::string& out);
        mesh_converter(const mesh_converter& other) = delete;
        mesh_converter(mesh_converter&& other) = delete;
        ~mesh_converter() noexcept;

        void parse();
        void serialize();

        mesh_converter& operator=(const mesh_converter& other) = delete;
        mesh_converter& operator=(mesh_converter&& other) = delete;
    };
}

#endif

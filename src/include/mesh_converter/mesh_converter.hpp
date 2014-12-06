#ifndef __DEVTOOLS_MESH_CONVERTER_HPP__
#define __DEVTOOLS_MESH_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <tinyxml2.h>

namespace devtools {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };

    struct submesh {
        size_t index_count;
        size_t offset;
    };

    using index = unsigned int;
    using material = std::string;

    class mesh_converter {
        std::vector<submesh> submeshes_;
        std::vector<vertex> vertices_;
        std::vector<index> indices_;
        std::vector<material> materials_;
        tinyxml2::XMLDocument is_;
        std::ofstream os_;
    public:
        mesh_converter(const std::string& in, const std::string& out);
        mesh_converter(const mesh_converter& other) = delete;
        mesh_converter(mesh_converter&& other) = delete;
        ~mesh_converter() noexcept;

        void parse(bool collison_geometry);
        void serialize(bool collison_geometry);

        mesh_converter& operator=(const mesh_converter& other) = delete;
        mesh_converter& operator=(mesh_converter&& other) = delete;
    };
}

#endif

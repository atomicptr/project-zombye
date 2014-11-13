#ifndef __DEVTOOLS_MESH_CONVERTER_HPP__
#define __DEVTOOLS_MESH_CONVERTER_HPP__

#include <fstream>
#include <vector>

#include <glm/glm.hpp>

namespace devtools {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
    };

    class mesh_converter {
        std::vector<vertex> vertices_;
        std::ifstream is_;
        std::ofstream os_;
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

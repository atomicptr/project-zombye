#ifndef __DEVTOOLS_MESH_CONVERTER_HPP__
#define __DEVTOOLS_MESH_CONVERTER_HPP__

#include <string>

#include <glm/glm.hpp>
#include <json/json.h>

namespace devtools {
    struct header {
        const unsigned int magic = 0x31424D5A;
        size_t vertex_count = 0;
        size_t index_count = 0;
        size_t submesh_count = 0;
    };

    struct vertex {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    struct skin {
        glm::ivec4 indices;
        glm::vec4 weights;
    };

    struct submesh {
        size_t index_count = 0;
        size_t offset = 0;
        unsigned long diffuse = 0;
        unsigned long normal = 0;
        unsigned long material = 0;
    };

    class mesh_converter {
    private:
        Json::Reader reader_;
        Json::Value root_;
        std::string output_path_;

    public:
        mesh_converter(const std::string& input_file, const std::string& output_path);
        ~mesh_converter() = default;

        mesh_converter(const mesh_converter& rhs) = delete;
        mesh_converter& operator=(const mesh_converter& rhs) = delete;

        mesh_converter(mesh_converter&& rhs) = delete;
        mesh_converter& operator=(mesh_converter&& rhs) = delete;

        void run();
    };
}

#endif

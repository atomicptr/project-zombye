#ifndef __DEVTOOLS_MESH_CONVERTER_HPP__
#define __DEVTOOLS_MESH_CONVERTER_HPP__

#include <string>

#include <glm/glm.hpp>
#include <json/json.h>

namespace devtools {
    struct header {
        const uint32_t magic = 0x31424D5A;
        uint64_t vertex_count = 0;
        uint64_t index_count = 0;
        uint64_t submesh_count = 0;
    };

    struct collision_header {
        const uint32_t magic = 0x3142435A;
        uint64_t vertex_count = 0;
        uint64_t index_count = 0;
    };

    struct vertex {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
        glm::vec3 tangent;
    };

    struct skin {
        glm::ivec4 indices;
        glm::vec4 weights;
    };

    struct submesh {
        uint64_t index_count = 0;
        uint64_t offset = 0;
        uint64_t diffuse = 0;
        uint64_t normal = 0;
        uint64_t material = 0;
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

        void run(bool collision_meshes = false);
    };
}

#endif

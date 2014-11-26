#ifndef __ZOMBYE_ANIMATION_CONVERTER_HPP__
#define __ZOMBYE_ANIMATION_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace devtools {
    struct bone {
        unsigned int id;
        glm::mat4 transformation;
    };

    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
        glm::ivec4 index;
        glm::vec4 weight;
        unsigned int link_count = 0;
    };

    struct submesh {
        size_t index_count;
        size_t offset;
        size_t base_vertex;
    };

    struct vector_key {
        float time;
        glm::vec3 value;
    };

    struct quaternion_key {
        float time;
        glm::quat value;
    };

    struct node {
        unsigned int bone = 0;
        std::vector<vector_key> position_keys;
        std::vector<quaternion_key> rotation_keys;
        std::vector<vector_key> scaling_keys;
    };

    struct animation {
        std::string name;
        float duration;
        float tps;
        std::vector<node> nodes;
    };

    using index = unsigned int;
    using material = std::string;

    class animation_converter {
        std::string animation_name;
        std::vector<bone> bones_;
        std::unordered_map<std::string, unsigned int> bone_ids_;
        std::vector<submesh> submeshes_;
        std::vector<vertex> vertices_;
        std::vector<index> indices_;
        std::vector<material> materials_;
        animation animation_;
        Assimp::Importer is_;
        std::ofstream os_;
        const aiScene* scene_;
    public:
        animation_converter(const std::string& in, const std::string& out);
        animation_converter(const animation_converter& other) = delete;
        animation_converter(animation_converter&& other) = delete;
        ~animation_converter() noexcept;

        void parse(const std::string& name);
        void serialize();

        animation_converter& operator=(const animation_converter& other) = delete;
        animation_converter& operator=(animation_converter&& other) = delete;
    };
}

#endif
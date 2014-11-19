#ifndef __ZOMBYE_ANIMATION_CONVERTER_HPP__
#define __ZOMBYE_ANIMATION_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace devtools {
    struct bone {
        int id;
        int parent;
    };

    struct link {
        int bone;
        float weight;
    };

    struct bone_pose {
        int id;
        glm::vec3 loc_trans;
        glm::quat loc_rot;
    };

    struct vertex {
        glm::vec3 pos;
        glm::vec3 nor;
        glm::vec2 tex;
        int parent_bone;
        int link_count;
        link links[4];
    };

    using frame = std::vector<bone_pose>;

    class animation_converter {
        std::vector<bone> bones_;
        std::vector<frame> frames_;
        std::unordered_map<std::string, std::vector<vertex>> submeshes_;
        std::ifstream is_;
        std::ofstream os_;
    public:
        animation_converter(const std::string& in, const std::string& out);
        animation_converter(const animation_converter& other) = delete;
        animation_converter(animation_converter&& other) = delete;
        ~animation_converter() noexcept;

        void parse();
        void serialize();

        animation_converter& operator=(const animation_converter& other) = delete;
        animation_converter& operator=(animation_converter&& other) = delete;
    };
}

#endif
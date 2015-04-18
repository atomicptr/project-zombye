#ifndef __ZOMBYE_ANIMATION_CONVERTER_HPP__
#define __ZOMBYE_ANIMATION_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <json/json.h>

namespace devtools {
    struct header {
        const unsigned int magic = 0x3142415A;
        size_t bone_count = 0;
    };

    struct bone {
        int id;
        int parent;
        glm::mat4 relative_transform;
        glm::mat4 absolute_transform;
    };

    struct keyframe {
        float time;
        glm::vec3 translate;
        glm::quat rotate;
        glm::vec3 scale;
    };

    struct track {
        int id;
        int parent;
        std::vector<keyframe> keyframes;
    };

    struct animation {
        std::string name;
        float length;
        std::unordered_map<int, track> tracks;
    };

    class animation_converter {
    private:
        Json::Reader reader_;
        Json::Value root_;
        std::string output_path_;

    public:
        animation_converter(const std::string& input_file, const std::string& output_path);
        ~animation_converter() = default;

        animation_converter(const animation_converter& other) = delete;
        animation_converter& operator=(const animation_converter& other) = delete;

        animation_converter(animation_converter&& other) = delete;
        animation_converter& operator=(animation_converter&& other) = delete;

        void run();
    };
}

#endif
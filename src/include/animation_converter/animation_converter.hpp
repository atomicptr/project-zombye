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
        const uint32_t magic = 0x3142415A;
        int64_t size = 0;
        int64_t bone_count = 0;
        int64_t animation_count = 0;
    };

    struct node {
        int32_t id;
        int32_t children_count;
    };

    struct bone {
        int32_t id;
        int32_t parent;
        glm::mat4 relative_transform;
        glm::mat4 absolute_transform;
    };

    struct translation_keyframe {
        float time;
        glm::vec3 translate;
    };

    struct rotation_keyframe {
        float time;
        glm::quat rotate;
    };

    struct scale_keyframe {
        float time;
        glm::vec3 scale;
    };

    struct track {
        int32_t id;
        int32_t parent;
        int64_t tkey_count;
        int64_t qkey_count;
        int64_t skey_count;
    };

    struct animation {
        std::string name;
        float length;
        int64_t track_count;
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
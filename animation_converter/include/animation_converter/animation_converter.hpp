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
        uint64_t size = 0;
        uint64_t bone_count = 0;
        uint64_t animation_count = 0;
    };

    struct node {
        uint32_t id;
        uint32_t children_count;
    };

    struct bone {
        uint32_t id;
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
        uint32_t id;
        int32_t parent;
        uint64_t tkey_count;
        uint64_t qkey_count;
        uint64_t skey_count;
    };

    struct animation {
        std::string name;
        float length;
        uint64_t track_count;
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

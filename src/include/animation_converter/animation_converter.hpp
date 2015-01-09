#ifndef __ZOMBYE_ANIMATION_CONVERTER_HPP__
#define __ZOMBYE_ANIMATION_CONVERTER_HPP__

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <tinyxml2.h>

namespace devtools {
    struct bone {
        unsigned int id;
        glm::mat4 transform;
    };

    struct keyframe {
        float time;
        glm::vec3 translate;
        glm::quat rotate;
        glm::vec3 scale;
    };

    struct track {
        unsigned int id;
        std::vector<keyframe> keyframes;
    };

    struct animation {
        std::string name;
        float length;
        std::vector<track> tracks;
    };

    class animation_converter {
        std::vector<bone> bones_;
        std::vector<animation> animations_;
        tinyxml2::XMLDocument is_;
        std::ofstream os_;
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
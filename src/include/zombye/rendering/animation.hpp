#ifndef __ZOMBYE_ANIMATION_HPP__
#define __ZOMBYE_ANIMATION_HPP__

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace zombye {
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

    class rendering_system;

    class animation {
        std::string name_;
        float duration_;
        float tps_;
        std::vector<node> nodes_;

    public:
        animation(rendering_system& rendering_system, const std::vector<char>& data);
        animation(const animation& other) = delete;
        animation(animation&& other) = delete;
        ~animation() = default;
        animation& operator=(const animation& other) = delete;
        animation& operator=(animation&& other) = delete;
    };
}

#endif
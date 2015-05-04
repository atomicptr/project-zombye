#ifndef __ZOMBYE_SKELETON_HPP__
#define __ZOMBYE_SKELETON_HPP__

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace zombye {
    class rendering_system;
}

namespace zombye {
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
        std::vector<translation_keyframe> tkeys;
        std::vector<rotation_keyframe> qkeys;
        std::vector<scale_keyframe> skeys;
    };

    struct animation {
        std::string name;
        float length;
        std::unordered_map<int, track> tracks;
    };

    struct node {
        int32_t id;
        int32_t children_count;
    };

    class skeleton {
        struct header {
            uint32_t magic;
            int64_t size;
            int64_t bone_count;
            int64_t animation_count;
        };

        friend class animation_component;

        std::vector<bone> bones_;
        std::unordered_multimap<int, int> bone_hierachy_;
        std::unordered_map<std::string, zombye::animation> animations_;
    public:
        skeleton(rendering_system& rendering_system, const std::vector<char>& source, const std::string& file_name) noexcept;
        skeleton(const skeleton& other) = delete;
        skeleton(skeleton&& other) noexcept = default;
        skeleton() = default;
        skeleton& operator=(const skeleton& other) = delete;
        skeleton& operator=(skeleton&& other) noexcept = default;

        const auto& bones() const noexcept {
            return bones_;
        }

        const auto& animation(const std::string& name) const {
            auto it = animations_.find(name);
            if (it == animations_.end()) {
                throw std::invalid_argument("no animation named " + name + " in skeleton");
            }
            return it->second;
        }
    };
}

#endif

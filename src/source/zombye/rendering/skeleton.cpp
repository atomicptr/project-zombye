#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/skeleton.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    skeleton::skeleton(rendering_system& rendering_system, const std::vector<char>& source) noexcept {
        auto data_ptr = source.data();

        auto bone_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0u; i < bone_count; ++i) {
            bones_.emplace_back(*reinterpret_cast<const bone*>(data_ptr));
            data_ptr += sizeof(bone);
        }

        auto animation_count = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0u; i < animation_count; ++i) {
            auto animation = zombye::animation{};

            auto name_size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            animation.name = std::string{data_ptr, name_size};
            data_ptr += name_size;

            animation.length = *reinterpret_cast<const float*>(data_ptr);
            data_ptr += sizeof(float);

            auto track_count = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            for (auto j = 0u; j < track_count; ++j) {
                auto track = zombye::track{};

                track.id = *reinterpret_cast<const int*>(data_ptr);
                data_ptr += sizeof(int);
                track.parent = *reinterpret_cast<const int*>(data_ptr);
                data_ptr += sizeof(int);

                auto keyframe_count = *reinterpret_cast<const size_t*>(data_ptr);
                data_ptr += sizeof(size_t);
                for (auto k = 0u; k < keyframe_count; ++k) {
                    auto keyframe = zombye::keyframe{};

                    keyframe.time = *reinterpret_cast<const float*>(data_ptr);
                    data_ptr += sizeof(float);

                    keyframe.translate = *reinterpret_cast<const glm::vec3*>(data_ptr);
                    data_ptr += sizeof(glm::vec3);

                    keyframe.rotate = glm::normalize(*reinterpret_cast<const glm::quat*>(data_ptr));
                    data_ptr += sizeof(glm::quat);

                    keyframe.scale = *reinterpret_cast<const glm::vec3*>(data_ptr);
                    data_ptr += sizeof(glm::vec3);

                    track.keyframes.emplace_back(keyframe);
                }

                animation.tracks.insert(std::make_pair(track.id, track));
            }

            animations_.insert(std::make_pair(animation.name, animation));
        }
    }
}

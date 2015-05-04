#include <stdexcept>

#include <zombye/rendering/rendering_system.hpp>
#include <zombye/rendering/skeleton.hpp>

#include <glm/gtx/string_cast.hpp>

namespace zombye {
    skeleton::skeleton(rendering_system& rendering_system, const std::vector<char>& source, const std::string& file_name) noexcept {
        auto data_ptr = source.data();

        auto head = *reinterpret_cast<const header*>(data_ptr);
        if (head.magic != 0x3142415A) {
            throw std::runtime_error(file_name + " is not a zombye skeleton file");
        }
        data_ptr += sizeof(header);

        if (head.size != source.size()) {
            throw std::runtime_error(file_name + " has not the apropriate size. expected size: "
                + std::to_string(source.size()) + " calculated size: " + std::to_string(head.size));
        }

        bones_.reserve(head.bone_count);
        for (auto i = 0ul; i < head.bone_count; ++i) {
            bones_.emplace_back(*reinterpret_cast<const bone*>(data_ptr));
            data_ptr += sizeof(bone);
        }

        for (auto i = 0ul; i < head.bone_count; ++i) {
            node n = *reinterpret_cast<const node*>(data_ptr);
            data_ptr += sizeof(node);
            for (auto j = 0; j < n.children_count; ++j) {
                int32_t id = *reinterpret_cast<const int32_t*>(data_ptr);
                data_ptr += sizeof(int32_t);
                bone_hierachy_.emplace(std::make_pair(int(i), id));
            }
        }

        for (auto i = 0ul; i < head.animation_count; ++i) {
            zombye::animation a;

            a.name = std::string{data_ptr};
            data_ptr += 20;

            a.length = *reinterpret_cast<const float*>(data_ptr);
            data_ptr += sizeof(float);

            auto track_count = *reinterpret_cast<const int64_t*>(data_ptr);
            data_ptr += sizeof(int64_t);
            a.tracks.reserve(track_count);
            for (auto j = 0ul; j < track_count; ++j) {
                track t;

                t.id = *reinterpret_cast<const int32_t*>(data_ptr);
                data_ptr += sizeof(int32_t);

                t.parent = *reinterpret_cast<const int32_t*>(data_ptr);
                data_ptr += sizeof(int32_t);

                auto tkey_count = *reinterpret_cast<const int64_t*>(data_ptr);
                data_ptr += sizeof(int64_t);
                t.tkeys.reserve(tkey_count);

                auto qkey_count = *reinterpret_cast<const int64_t*>(data_ptr);
                data_ptr += sizeof(int64_t);
                t.qkeys.reserve(qkey_count);

                auto skey_count = *reinterpret_cast<const int64_t*>(data_ptr);
                data_ptr += sizeof(int64_t);
                t.skeys.reserve(skey_count);

                for (auto k = 0ul; k < tkey_count; ++k) {
                    t.tkeys.emplace_back(*reinterpret_cast<const translation_keyframe*>(data_ptr));
                    data_ptr += sizeof(translation_keyframe);
                }

                for (auto k = 0ul; k < qkey_count; ++k) {
                    t.qkeys.emplace_back(*reinterpret_cast<const rotation_keyframe*>(data_ptr));
                    data_ptr += sizeof(rotation_keyframe);
                }

                for (auto k = 0ul; k < skey_count; ++k) {
                    t.skeys.emplace_back(*reinterpret_cast<const scale_keyframe*>(data_ptr));
                    data_ptr += sizeof(scale_keyframe);
                }

                a.tracks.insert(std::make_pair(t.id, t));
            }
            animations_.insert(std::make_pair(a.name, a));
        }
    }
}

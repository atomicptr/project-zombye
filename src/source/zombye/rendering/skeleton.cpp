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
        auto bone_size = head.bone_count * sizeof(bone);
        auto size = sizeof(header)
            + bone_size;

        if (size != source.size()) {
            throw std::runtime_error(file_name + " has not the apropriate size. expected size: "
                + std::to_string(source.size()) + " calculated size: " + std::to_string(size));
        }
        data_ptr += sizeof(header);

        bones_.resize(head.bone_count);
        for (auto i = 0ul; i < head.bone_count; ++i) {
            bones_.emplace_back(*reinterpret_cast<const bone*>(data_ptr));
            data_ptr += sizeof(bone);
        }
    }
}

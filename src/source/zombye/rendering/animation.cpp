#include <string>

#include <zombye/rendering/animation.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    animation::animation(rendering_system&, const std::vector<char>& data) {
        auto data_ptr = data.data();
        auto size = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        name_ = std::string{data_ptr, size};
        data_ptr += size;
        duration_ = *reinterpret_cast<const float*>(data_ptr);
        data_ptr += sizeof(float);
        tps_ = *reinterpret_cast<const float*>(data_ptr);
        data_ptr += sizeof(float);
        size = *reinterpret_cast<const size_t*>(data_ptr);
        data_ptr += sizeof(size_t);
        for (auto i = 0u; i < size; ++i) {
            node n;
            n.bone = *reinterpret_cast<const unsigned int*>(data_ptr);
            data_ptr += sizeof(unsigned int);

            auto pk_size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            for (auto j = 0u; j < pk_size; ++j) {
                n.position_keys.emplace_back(*reinterpret_cast<const vector_key*>(data_ptr));
                data_ptr += sizeof(vector_key);
            }

            auto rk_size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            for (auto j = 0u; j < rk_size; ++j) {
                n.rotation_keys.emplace_back(*reinterpret_cast<const quaternion_key*>(data_ptr));
                data_ptr += sizeof(quaternion_key);
            }

            auto sk_size = *reinterpret_cast<const size_t*>(data_ptr);
            data_ptr += sizeof(size_t);
            for (auto j = 0u; j < sk_size; ++j) {
                n.scaling_keys.emplace_back(*reinterpret_cast<const vector_key*>(data_ptr));
                data_ptr += sizeof(vector_key);
            }

            nodes_.emplace_back(n);
        }
    }
}
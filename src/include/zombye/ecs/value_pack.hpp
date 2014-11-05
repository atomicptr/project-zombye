#ifndef __ZOMBYE_VALUE_PACK_HPP__
#define __ZOMBYE_VALUE_PACK_HPP__

#include <memory>
#include <vector>

#include <zombye/ecs/typed_property.hpp>
#include <zombye/ecs/typed_value.hpp>

namespace zombye {
    class value_pack {
        std::vector<std::unique_ptr<abstract_value>> values_;
    public:
        value_pack() noexcept = default;
        value_pack(const value_pack& other) = delete;
        value_pack(value_pack&& other) = delete;
        template <typename type>
        void emplace(const typed_property<type>& assigner, type value) {
            values_.emplace(std::unique_ptr<typed_value<type>>(new typed_value<type>(assigner, value)));
        }

        auto begin() const {
            return values_.begin();
        }

        auto end() const {
            return values_.end();
        }

        const auto& get() const {
            return values_;
        }
        value_pack& operator= (const value_pack& other) = delete;
        value_pack& operator= (value_pack&& other) = delete;
    };
}

#endif
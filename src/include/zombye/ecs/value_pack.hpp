#ifndef __ZOMBYE_VALUE_PACK_HPP__
#define __ZOMBYE_VALUE_PACK_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/typed_property.hpp>
#include <zombye/ecs/typed_value.hpp>

namespace zombye {
    class value_pack {
        using values = std::vector<std::unique_ptr<abstract_value>>;
        std::string name_;
        values values_;
    public:
        value_pack(const std::string& name) noexcept : name_(name) { }
        value_pack(const value_pack& other) = delete;
        value_pack(value_pack&& other) = delete;
        template <typename... arguments>
        void emplace_back(arguments... args) {
            values_.emplace_back(std::forward<arguments>(args)...);
        }

        const std::string& name() const  {
            return name_;
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
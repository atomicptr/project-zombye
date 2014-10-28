#ifndef __ZOMBYE_REFLECTIVE_HPP__
#define __ZOMBYE_REFLECTIVE_HPP__

#include <memory>
#include <string>
#include <typeinfo>

#include <zombye/ecs/property.hpp>
#include <zombye/ecs/rtti.hpp>
#include <zombye/utils/demangle.hpp>

namespace zombye {
    class nulltype {
    public:
        static std::shared_ptr<rtti> type_rtti() noexcept {
            return std::shared_ptr<rtti>();
        }
    };

    template <typename type, typename base_type>
    class reflective : public base_type {
    protected:
        static std::shared_ptr<class rtti> rtti_;
    public:
        reflective() noexcept = default;
        reflective(const reflective& other) = delete;
        reflective(reflective&& other) = delete;
        static type* create(game& game, entity& owner) {
            return new type(game, owner);
        }
        static void register_reflection() noexcept { }
        template <typename property_type>
        static void register_property(const std::string& name,
            decltype(property<type, property_type>::getter) getter,
            decltype(property<type, property_type>::setter) setter) {
            std::shared_ptr<property<type, property_type>> property = std::make_shared(name, getter, setter);
            type::type_rtti()->properties().push_back(property);
        }
        static std::shared_ptr<class rtti> type_rtti() noexcept {
            return rtti_;
        }
        virtual std::shared_ptr<class rtti> rtti() noexcept {
            return rtti_;
        }
        reflective& operator= (const reflective& other) = delete;
        reflective& operator= (reflective&& other) = delete;
    };
    template <typename type, typename base_type>
    std::shared_ptr<class rtti> reflective<type, base_type>::rtti_(std::make_shared<class rtti>(0, demangle(typeid(type).name()),
        base_type::type_rtti(), &type::create));
}

#endif

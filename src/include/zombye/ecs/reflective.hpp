#ifndef __ZOMBYE_REFLECTIVE_HPP__
#define __ZOMBYE_REFLECTIVE_HPP__

#include <memory>
#include <string>
#include <typeinfo>

#include <zombye/ecs/property.hpp>
#include <zombye/ecs/rtti.hpp>
#include <zombye/utils/demangle.hpp>

namespace zombye {
    class entity;
    class game;
    class nulltype {
    public:
        nulltype(game& game, entity& owner) noexcept { }
        static rtti* type_rtti() noexcept {
            return nullptr;
        }
    };

    template <typename type, typename base_type>
    class reflective : public base_type {
        static class rtti rtti_;
    public:
        reflective(game& game, entity& owner) noexcept : base_type(game, owner) { }
        reflective(const reflective& other) = delete;
        reflective(reflective&& other) = delete;
        static type* create(game& game, entity& owner) {
            return new type(game, owner);
        }
        static void register_reflection() { }
        template <typename property_type>
        static void register_property(const std::string& name,
            typename property<type, property_type>::getter_type getter,
            typename property<type, property_type>::setter_type setter) {
            type_rtti()->properties().emplace_back(new property<type, property_type>(name, getter, setter));
        }
        static class rtti* type_rtti() noexcept {
            return &rtti_;
        }
        virtual class rtti* rtti() noexcept {
            return type_rtti();
        }
        reflective& operator= (const reflective& other) = delete;
        reflective& operator= (reflective&& other) = delete;
    };
    template <typename type, typename base_type>
    rtti reflective<type, base_type>::rtti_(demangle(typeid(type).name()), base_type::type_rtti(), (factory_function)type::create,
                (reflection_function)type::register_reflection);
}

#endif

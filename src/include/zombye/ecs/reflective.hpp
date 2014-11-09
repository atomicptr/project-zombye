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
        friend void rtti_manager::register_type(rtti*);
        static void register_reflection() { }
    public:
        reflective(game& game, entity& owner) noexcept : base_type(game, owner) { }
        reflective(const reflective& other) = delete;
        reflective(reflective&& other) = delete;
        static type* create(game& game, entity& owner) {
            return new type(game, owner);
        }
        template <typename property_type>
        static void register_property(const std::string& name,
            typename property<type, property_type>::getter_type getter,
            typename property<type, property_type>::setter_type setter) {
            type_rtti()->emplace_back(new property<type, property_type>(name, getter, setter));
        }
        static zombye::rtti* type_rtti() noexcept {
            static zombye::rtti rtti_(demangle(typeid(type).name()), base_type::type_rtti(),
                (rtti::factory)type::create, (rtti::reflection)type::register_reflection);
            return &rtti_;
        }
        virtual zombye::rtti* rtti() noexcept {
            return type_rtti();
        }
        reflective& operator= (const reflective& other) = delete;
        reflective& operator= (reflective&& other) = delete;
    };
}

#endif

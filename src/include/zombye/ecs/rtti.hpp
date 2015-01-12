#ifndef __ZOMBYE_RTTI_HPP__
#define __ZOMBYE_RTTI_HPP__

#include <memory>
#include <string>
#include <vector>

#include <zombye/ecs/abstract_property.hpp>
#include <zombye/ecs/rtti_manager.hpp>

namespace zombye {
    class component;
    class entity;
    class game;
    class rtti {
    public:
        using factory = component* (*)(game&, entity&);
        using reflection = void (*)();
        using property_list = std::vector<std::unique_ptr<abstract_property>>;
    private:
        friend void rtti_manager::register_type(rtti*);
        static unsigned long id_generator_;
        unsigned long type_id_;
        std::string type_name_;
        factory factory_;
        reflection reflection_;
        property_list properties_;
    public:
        rtti(const std::string& type_name, factory factory, reflection reflection) noexcept;
        void emplace_back(abstract_property* property) {
            properties_.emplace_back(property);
        }
        unsigned long type_id() const noexcept {
            return type_id_;
        }
        const std::string& type_name() const noexcept {
            return type_name_;
        }
        const factory& ctor() const noexcept {
            return factory_;
        }
        const property_list& properties() const noexcept {
            return properties_;
        }
    };
}

#endif

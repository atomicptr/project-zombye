#ifndef __ZOMBYE_TYPED_PROPERTY_HPP__
#define __ZOMBYE_TYPED_PROPERTY_HPP__

#include <zombye/ecs/abstract_property.hpp>

namespace zombye {
    class component;
    template <typename value_type>
    class typed_property : public abstract_property {
    public:
        typed_property(const std::string& name) noexcept : abstract_property(name) { }
        virtual property_types type() const noexcept {
            return property_type<value_type>::type_id();
        }
        virtual value_type value(component* owner) = 0;
        virtual void value(component* owner, value_type value) = 0;
    };
}

#endif

#ifndef __ZOMBYE_PROPERTY_HPP__
#define __ZOMBYE_PROPERTY_HPP__

#include <zombye/ecs/typed_property.hpp>

namespace zombye {
    class component;
    template <typename owner_type, typename value_type>
    class property : public typed_property<value_type> {
        typedef value_type (owner_type::*getter_type)();
        typedef void (owner_type::*setter_type)(value_type value);
    protected:
        getter_type getter;
        setter_type setter;
    public:
        property(const std::string& name) noexcept : typed_property<value_type>(name) { }
        virtual value_type value(component* owner) const noexcept {
            return owner->*getter();
        }
        virtual void value(component* owner, value_type value) noexcept {
            if (!setter) {
                // TODO: Appropriate errror handling
                return;
            }
            owner->*setter(value);
        }
    };
}

#endif

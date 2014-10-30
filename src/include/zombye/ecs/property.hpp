#ifndef __ZOMBYE_PROPERTY_HPP__
#define __ZOMBYE_PROPERTY_HPP__

#include <zombye/ecs/typed_property.hpp>

namespace zombye {
    class component;
    template <typename owner_type, typename value_type>
    class property : public typed_property<value_type> {
    public:
        typedef value_type (owner_type::*getter_type)();
        typedef void (owner_type::*setter_type)(value_type value);
    protected:
        getter_type getter_;
        setter_type setter_;
    public:
        property(const std::string& name, getter_type getter, setter_type setter) noexcept
        : typed_property<value_type>(name), getter_(getter), setter_(setter) { }
        virtual value_type value(component* owner) {
            return (static_cast<owner_type*>(owner)->*getter_)();
        }
        virtual void value(component* owner, value_type value) {
            if (!setter_) {
                // TODO: Appropriate errror handling
                return;
            }
            (static_cast<owner_type*>(owner)->*setter_)(value);
        }
    };
}

#endif

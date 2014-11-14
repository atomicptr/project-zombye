#ifndef __ZOMBYE_PROPERTY_HPP__
#define __ZOMBYE_PROPERTY_HPP__

#include <zombye/ecs/typed_property.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
    class component;
    template <typename owner_type, typename value_type>
    class property : public typed_property<value_type> {
    public:
        typedef value_type (owner_type::*getter_type)() const;
        typedef void (owner_type::*setter_type)(const value_type& value);
    private:
        getter_type getter_;
        setter_type setter_;
    public:
        property(const std::string& name, getter_type getter, setter_type setter) noexcept
        : typed_property<value_type>(name), getter_(getter), setter_(setter) { }
        virtual value_type value(component* owner) const {
            return (static_cast<owner_type*>(owner)->*getter_)();
        }
        virtual void set_value(component* owner, const value_type& value) {
            if (!setter_) {
                log(LOG_WARNING, "property " + this->name_ + " has no setter");
                return;
            }
            (static_cast<owner_type*>(owner)->*setter_)(value);
        }
    };
}

#endif

#ifndef __ZOMBYE_TYPED_VALUE_HPP__
#define __ZOMBYE_TYPED_VALUE_HPP__

#include <zombye/ecs/abstract_value.hpp>
#include <zombye/ecs/typed_property.hpp>

namespace zombye {
    class component;
    template <typename type>
    class typed_value : public abstract_value {
        const typed_property<type>& assigner_;
        type value_;
    public:
        typed_value(const typed_property<type>& assigner, const type& value) noexcept
        : assigner_(assigner), value_(value) { }
        typed_value(const typed_value& other) = delete;
        typed_value(typed_value&& other) = delete;
        void assign(component* owner) {
            assigner_.set_value(owner, value_);
        }
        typed_value& operator= (const typed_value& other) = delete;
        typed_value& operator= (typed_value&& other) = delete;
    };
}

#endif
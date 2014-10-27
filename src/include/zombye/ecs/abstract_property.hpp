#ifndef __ZOMBYE_ABSTRACT_PROPERTY_HPP__
#define __ZOMBYE_ABSTRACT_PROPERTY_HPP__

#include <string>

#include <zombye/ecs/property_types.hpp>

namespace zombye {
    class abstract_property {
    protected:
        std::string name_;
    public:
        abstract_property(const std::string& name) noexcept : name_(name) { }
        const std::string& name() const noexcept {
            return name_;
        }
        virtual property_types type() const = 0;
    };
}

#endif

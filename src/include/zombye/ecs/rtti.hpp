#ifndef __ZOMBYE_RTTI_HPP__
#define __ZOMBYE_RTTI_HPP__

#include <memory>
#include <string>

namespace zombye {
    class abstract_property;
    class component;
    class entity;
    class game;
    typedef component*(*factory_function)(game&, entity&);
    typedef void (*reflection_function)();
    class rtti {
        unsigned long type_id_;
        std::string type_name_;
        rtti* base_rtti_;
        factory_function factory_;
        std::vector<std::shared_ptr<abstract_property>> properties_;
    public:
        rtti(unsigned long type_id, const std::string& type_name,rtti* base_rtti, factory_function factory,
            reflection_function reflection) noexcept
        : type_id_(type_id), type_name_(type_name), base_rtti_(base_rtti), factory_(factory) {
            if (reflection) {
                reflection();
            }
        }
        unsigned long type_id() const noexcept {
            return type_id_;
        }
        const std::string& type_name() const noexcept {
            return type_name_;
        }
        rtti* base_rtti() const noexcept {
            return base_rtti_;
        }
        const factory_function factory() const noexcept {
            return factory_;
        }
        std::vector<std::shared_ptr<abstract_property>>& properties() noexcept {
            return properties_;
        }
    };
}

#endif

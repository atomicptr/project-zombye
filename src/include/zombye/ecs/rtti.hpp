#ifndef __ZOMBYE_RTTI_HPP__
#define __ZOMBYE_RTTI_HPP__

#include <memory>
#include <string>

namespace zombye {
    class abstract_property;
    class component;
    class entity;
    class game;
    class rtti {
        unsigned long type_id_;
        std::string type_name_;
        std::shared_ptr<rtti> base_rtti_;
        std::function<component*(game&, entity&)> factory_;
        std::vector<std::shared_ptr<abstract_property>> properties_;
    public:
        rtti(unsigned long type_id, const std::string& type_name, const std::shared_ptr<rtti>& base_rtti,
            std::function<component*(game&, entity&)> factory) noexcept
        : type_id_(type_id), type_name_(type_name), base_rtti_(base_rtti), factory_(factory) { }
        unsigned long type_id() const noexcept {
            return type_id_;
        }
        const std::string& type_name() const noexcept {
            return type_name_;
        }
        std::shared_ptr<const rtti> base_rtti() const noexcept {
            return base_rtti_;
        }
        const std::function<component*(game&, entity&)>& factory() const noexcept {
            return factory_;
        }
        const std::vector<std::shared_ptr<abstract_property>>& properties() const noexcept {
            return properties_;
        }
    };
}

#endif

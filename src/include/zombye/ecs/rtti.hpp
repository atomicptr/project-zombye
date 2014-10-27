#ifndef __ZOMBYE_RTTI_HPP__
#define __ZOMBYE_RTTI_HPP__

#include <memory>
#include <string>

namespace zombye {
    class component;
    class entity;
    class game;
    struct rtti {
        unsigned long type_id;
        std::string type_name;
        std::shared_ptr<rtti> base_rtti;
        std::function<component*(game&, entity&)> factory;
        rtti(unsigned long type_id, const std::string& type_name, const std::shared_ptr<rtti>& base_rtti,
            std::function<component*(game&, entity&)> factory) noexcept
        : type_id(type_id), type_name(type_name), base_rtti(base_rtti), factory(factory) { }
    };
}

#endif

#include <zombye/ecs/rtti.hpp>
#include <zombye/ecs/rtti_manager.hpp>
#include <iostream>

namespace zombye {
    unsigned long rtti::id_generator_ = 0;

    rtti::rtti(const std::string& type_name,rtti* base_rtti, factory factory, reflection reflection) noexcept
    : type_id_(++id_generator_), type_name_(type_name), base_rtti_(base_rtti), factory_(factory),
    reflection_(reflection) { }
}
#include <zombye/ecs/rtti.hpp>
#include <zombye/ecs/rtti_manager.hpp>

namespace zombye {
    rtti::rtti(unsigned long type_id, const std::string& type_name,rtti* base_rtti, factory_function factory,
        reflection_function reflection) noexcept
    : type_id_(type_id), type_name_(type_name), base_rtti_(base_rtti), factory_(factory) {
        if (reflection) {
            reflection();
        }
        rtti_manager::register_type(this);
    }
}
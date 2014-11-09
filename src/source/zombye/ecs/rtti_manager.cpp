#include <zombye/ecs/rtti.hpp>
#include <zombye/ecs/rtti_manager.hpp>

namespace zombye {
    std::unordered_map<std::string, rtti*>& rtti_manager::rttis() {
        static std::unordered_map<std::string, rtti*> rttis;
        return rttis;
    }
    void rtti_manager::register_type(rtti* type_info) {
        if (!type_info) {
            return;
        }
        auto pos = rttis().find(type_info->type_name());
        if (pos == rttis().end()) {
            type_info->reflection_();
            rttis().insert(std::make_pair(type_info->type_name(), type_info));
        }
    }
    rtti* rtti_manager::type_info(const std::string& name) {
        auto pos = rttis().find(name);
        if (pos != rttis().end()) {
            return rttis()[name];
        }
        return nullptr;
    }
}
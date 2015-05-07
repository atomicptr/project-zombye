#ifndef __ZOMBYE_ASSIGN_HPP__
#define __ZOMBYE_ASSIGN_HPP__

#include <memory>
#include <string>

#include <json/json.h>

#include <zombye/ecs/rtti.hpp>

namespace zombye {
    class abstract_property;
}

namespace zombye {
        std::string to_string(const Json::Value& value);
        std::unique_ptr<value_pack> assign_values(const std::string& name, const Json::Value& object,
        const rtti::property_list& properties);
        std::unique_ptr<abstract_value> assign_value(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_bool(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_int(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_ivec2(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_ivec3(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_ivec4(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_float(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_vec2(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_vec3(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_vec4(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_quat(abstract_property* property, const Json::Value& wrapped_value);
        std::unique_ptr<abstract_value> assign_string(abstract_property* property, const Json::Value& wrapped_value);
}

#endif
